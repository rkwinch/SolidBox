#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <sstream>
#include "Sphere.h"
#include "CurvedSurface.h"
#include "ConnectionChannel.h"
#include "Utility.h"
#include "Menu.h"
#include "Speech.h"

const int Sphere::m_nSurfaces = 1;
int Sphere::m_nNameIDCounter = 0;
std::vector<std::shared_ptr<Sphere>> Sphere::m_shapeVec;

// parameterized constructor
Sphere::Sphere(double radius)
{
	m_nNumOfSurfaces = 1;
	m_stName = Utility::CreateUniqueName("sphere", m_nNameIDCounter);
	std::set<std::shared_ptr<Surface>> surfaceSet;

	//making 1 surface to go with the sphere
	for (int i = 0; i < Sphere::m_nSurfaces; ++i)
	{
		std::shared_ptr<CurvedSurface> surface = std::make_shared<CurvedSurface>(radius, &m_channel);
		surfaceSet.insert(surface);
	}

	m_channel.Connect(surfaceSet);

	//if all planes inserted correctly, then a proper ConnectionChannel has been made
	m_dRadius = radius;
	CalcSA();
	CalcVol();
	m_bHasConnection = true;
}

// destructor
Sphere::~Sphere()
{
	// don't need to do anything here since memory is handled via smart pointers
}

// copy constructor
Sphere::Sphere(const Sphere& other) : Shape(other)
{
	// don't change name
	if (m_stName.length() == 0)
	{
		m_stName = Utility::CreateUniqueName("sphere", m_nNameIDCounter);
	}

	m_nNumOfSurfaces = other.m_nNumOfSurfaces;
	m_dRadius = other.m_dRadius;
	m_bHasConnection = other.m_bHasConnection;
	m_channel.SetShape(this);
	CalcSA();
	CalcVol();
}

// since it was requested that old object be discarded afterwards, this is
// more like a move.  That is why this is not const. '=' is a move.
Sphere& Sphere::operator=(Sphere &sphere)
{
	m_dRadius = sphere.m_dRadius;
	m_channel = sphere.m_channel;
	m_bHasConnection = sphere.m_bHasConnection;
	CalcSA();
	CalcVol();
	sphere.Delete(); //**deleting items on right side of = operator**
	return *this;
}

void Sphere::Delete()
{
	std::vector<std::shared_ptr<Sphere>>::iterator shapeVecItr = m_shapeVec.begin();
	// [&] is take by reference, arg type is shared ptr of surface type (solidbox or sphere at this point), return type is bool, 
	// predicate is check if the shapes are equivalent (same name by == operator)
	shapeVecItr = std::find_if(m_shapeVec.begin(), m_shapeVec.end(), [&](std::shared_ptr<Sphere> shape)->bool {return *shape == *this; });

	if (shapeVecItr == m_shapeVec.end())
	{
		Utility::Display("Cannot delete Shape.  Shape not found\n");
		return;
	}

	m_channel.Disconnect(); // setting surfaces in surfaceSet to null
	m_shapeVec.erase(shapeVecItr); // removing item from vector
}

void Sphere::CalcVol()
{
	m_dVolume = 4.0 / 3.0 * CurvedSurface::PI * pow(m_dRadius, 3.0);
}

void Sphere::CalcSA()
{
	m_dSurfaceArea = 4.0 * CurvedSurface::PI * pow(m_dRadius, 2.0);
}

double Sphere::GetRadius()
{
	return m_dRadius;
}

std::set<std::shared_ptr<Surface>> Sphere::GetSurfacesCopy()
{
	std::set<std::shared_ptr<Surface>> surfaceSet;

	for (auto surface : m_channel.GetSurfaceSet()) // should just be one surface
	{
		std::shared_ptr<CurvedSurface> copy = std::make_shared<CurvedSurface>(m_dRadius, &m_channel);
		surfaceSet.insert(copy);
	}
	return surfaceSet;
}

std::vector<std::shared_ptr<Sphere>> Sphere::GetShapeVec()
{
	return m_shapeVec;
}

void Sphere::Save(std::ofstream &outFile)
{
	outFile << m_stName << ";";
	outFile << m_dRadius << ";" << m_bHasConnection << ";";
	m_channel.Save(outFile); // takes care of channel member and its associated planes
	outFile << "\n";
}

void Sphere::Create()
{
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();
	std::string strInput = "";
	double dRadius = 0.0;
	std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
																   // with 0-1 decimals followed by a number (if present) while allowing spaces
	Utility::Display("What would you like the radius to be? (in mm)\n");
	Utility::Display("ex: 0.125\n");

	if (isSpeech)
	{
		do
		{
			dRadius = Speech::RetrieveDouble();

			if (dRadius <= 0.0)
			{
				Utility::Display("Invalid parameter.  Must be greater than zero.  Please try again.\n");
			}

		} while (dRadius <= 0.0);
	}
	else
	{
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		dRadius = std::stod(strInput); // converting string input into a double
	}


	while (dRadius == 0.0)
	{
		Utility::Display("Please input a value that is not 0\n");
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		dRadius = std::stod(strInput);
	}

	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(dRadius);
	m_shapeVec.push_back(sphere);
	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
}

std::string Sphere::PrintSolids()
{
	std::ostringstream stream;
	int count = 1;

	for (auto sphere : m_shapeVec)
	{
		stream << count << ") " << sphere->GetName() << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
		++count;
	}

	return stream.str();
}

std::string Sphere::PrintSolids(int &counter)
{
	std::ostringstream stream;

	for (auto &sphere : Sphere::m_shapeVec)
	{
		stream << counter++ << ") " << sphere->GetName() << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
	}

	return stream.str();
}

void Sphere::Load(std::vector<std::string>::iterator &itr)
{
	std::string stName = "";
	double dRadius = 0;
	bool bHasConnection = false;
	int nNumOfEdges = 0;

	//getting members for sphere
	stName = (*itr);
	itr++;
	dRadius = stod(*itr);
	itr++;
	bHasConnection = static_cast<bool>(stoi(*itr));
	itr++;

	//constructing a sphere with given radius and setting other params
	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(dRadius);
	sphere->SetName(stName);

	//getting connectionchannel name
	stName = (*itr);
	itr++;
	sphere->GetConnChannel()->SetName(stName);

	//getting and setting members for square planes
	for (auto surface : sphere->GetConnChannel()->GetSurfaceSet())
	{
		stName = (*itr);
		itr++;
		dRadius = stod(*itr);
		itr++;
		nNumOfEdges = stoi(*itr);

		// making cast to CurvedSurface* since it is currently Surface* (and doesn't know about radius, etc)
		// do this by casting the smart pointer to the CurvedSurface and using get() to get the raw ptr
		// using dynamic casting because want to make sure the pointer can be converted properly to its derived 
		// class from the base (base should actually be a child in the first place, but need set to work with 
		// all types of surfaces, so have set of surfaces where the surfaces are actually curvedSurfaces or rectPlanes
		// (shouldn't be able to insert a surface anyway since it is a base abstract class)
		//
		// example of when to use and not to use:
		//(1) 
		//MyBase *base = static_cast<MyBase*>(child);
		//MyChild *child = dynamic_cast<MyChild*>(base); // good
		//(2)
		//MyBase  *base = new MyBase();
		//MyChild *child = dynamic_cast<MyChild*>(base); // bad
		auto curvedSurfacePtr = dynamic_cast<CurvedSurface*>(surface.get());
		curvedSurfacePtr->SetName(stName);
	}

	m_shapeVec.push_back(sphere); // solid box object is completed now.
}

bool Sphere::Move()
{
	int nMoveFrom = 0;
	int nMoveTo = 0;
	Menu* menu = Menu::GetInstance();
	bool bIsSpeech = menu->GetIsSpeechFlag();
	int sphereVecSize = static_cast<int>(Sphere::m_shapeVec.size());
	auto shapeVecItr_To = Sphere::m_shapeVec.begin();
	auto shapeVecItr_From = Sphere::m_shapeVec.begin();
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' while allowing for whitespace

	Utility::Display(Sphere::PrintSolids());
	Utility::Display("\n");

	//------Get Sphere selections from user----------------
	do
	{
		//moveFrom Sphere:
		do
		{
			if (bIsSpeech)
			{
				Utility::Display("Please select the sphere you are moving FROM or say \"back\" to go to the main menu.\n");
				nMoveFrom = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("Please select the sphere you are moving FROM or press 'b' to go back to the main menu.\n");
				nMoveFrom = Utility::RetrieveVecInput(acceptableInputExpr, sphereVecSize);
			}

			if (nMoveFrom == -1) return false; // user elected to go back to main menu

			if ((nMoveFrom < 1) || (nMoveFrom > sphereVecSize))
			{
				Utility::Display("Invalid entry.  Please try again.\n");
			}

		} while ((nMoveFrom < 1) || (nMoveFrom > sphereVecSize));

		shapeVecItr_From = std::next(shapeVecItr_From, (nMoveFrom - 1));

		//moveTo Sphere:
		do
		{
			if (bIsSpeech)
			{
				Utility::Display("Please select the sphere you are moving TO or say \"back\" to go to the main menu.\n");
				nMoveTo = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("Please select the sphere you are moving TO or press 'b' to go back to the main menu.\n");
				nMoveTo = Utility::RetrieveVecInput(acceptableInputExpr, sphereVecSize);
			}

			if (nMoveTo == -1) return false; // user elected to go back to main menu

			if ((nMoveTo < 1) || (nMoveTo > sphereVecSize))
			{
				Utility::Display("Invalid entry.  Please try again.\n");
			}

		} while ((nMoveTo < 1) || (nMoveTo > sphereVecSize));

		shapeVecItr_To = std::next(shapeVecItr_To, (nMoveTo - 1));

		if (nMoveFrom == nMoveTo) //check if trying to move to the same shape
		{
			Utility::Display("You cannot move from and to the same Sphere.  Please try again.\n");
		}

	} while (nMoveFrom == nMoveTo);

	// It's OK to now move From into To
	**shapeVecItr_To = **shapeVecItr_From; // calls move constructor
	return true;
}

bool Sphere::Move(std::shared_ptr<Sphere> sphere)
{
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();
	int nMoveTo = 0;
	int counter = 1;
	std::ostringstream stream;
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' while allowing for whitespace
	int solidBoxVecSize = static_cast<int>(Sphere::m_shapeVec.size());
	auto shapeVecItr_From = std::find(Sphere::m_shapeVec.begin(), Sphere::m_shapeVec.end(), sphere);
	auto shapeVecItr_To = Sphere::m_shapeVec.begin();

	do
	{
		//moveTo cube:
		do
		{
			if (isSpeech)
			{
				Utility::Display("\nPlease select the sphere you are moving TO or say \"back\" to go to the main menu.\n\n");
				Utility::Display(Sphere::PrintSolids());
				nMoveTo = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("\nPlease select the sphere you are moving TO or press 'b' to go back to the main menu.\n\n");
				Utility::Display(Sphere::PrintSolids());
				nMoveTo = Utility::RetrieveVecInput(acceptableInputExpr, solidBoxVecSize);
			}

			if (nMoveTo == -1) return false; // user elected to go back to main menu

			if ((nMoveTo < 1) || (nMoveTo > solidBoxVecSize))
			{
				Utility::Display("Invalid entry.  Please try again.\n");
			}

		} while ((nMoveTo < 1) || (nMoveTo > solidBoxVecSize));

		shapeVecItr_To = std::next(shapeVecItr_To, (nMoveTo - 1));

		if (*shapeVecItr_To == *shapeVecItr_From) //check if trying to move to the same shape
		{
			Utility::Display("You cannot move from and to the same sphere.  Please try again.\n");
		}

	} while (*shapeVecItr_To == *shapeVecItr_From);

	// It's OK to now move From into To
	**shapeVecItr_To = **shapeVecItr_From;
	return true;
}

std::string Sphere::PrintShapeInfo()
{
	std::string output = "";
	std::string strHeader = "Sphere:";
	std::ostringstream stream;

	stream << Utility::PrintHeader(strHeader);
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Shape name:" << m_stName << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "hasConnection:" << m_bHasConnection << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Channel name:" << m_channel.GetName() << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Radius (mm):" << std::fixed << std::setprecision(3) << m_dRadius << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Surface Area (mm^2):" << m_dSurfaceArea << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Volume (mm^3):" << m_dVolume << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Number of surfaces:" << m_nNumOfSurfaces << std::endl;
	stream << std::endl;
	return output = stream.str();
}

std::string Sphere::PrintPlanesInfo()
{
	std::string output = "";
	std::ostringstream stream;

	stream << Utility::PrintHeader("Surfaces:");

	for (auto plane : m_channel.GetSurfaceSet())
	{
		auto spherePtr = dynamic_cast<CurvedSurface*>(plane.get());
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Plane name:" << spherePtr->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Associated shape name:" << spherePtr->GetConnChannel()->GetShape()->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Associated channel name:" << spherePtr->GetConnChannel()->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Number of edges:" << spherePtr->GetNumOfEdges() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Radius (mm):" << std::fixed << std::setprecision(3) << spherePtr->GetRadius() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Area:" << std::fixed << std::setprecision(3) << spherePtr->GetArea() << std::endl;
		stream << std::endl;
	}

	return output = stream.str();
}