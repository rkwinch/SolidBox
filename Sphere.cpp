#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <iomanip>
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
Sphere::Sphere(Sphere& other)
{
	// don't change name
	m_dRadius = other.m_dRadius;
	this->CalcSA();
	this->CalcVol();
	m_channel = other.m_channel;
	m_bHasConnection = other.m_bHasConnection; // flag for checking if the SolidBox has a connection
}

// since it was requested that old object be discarded afterwards, this is
// more like a move.  That is why this is not const. '=' is a move.
Sphere& Sphere::operator=(Sphere &sphere)
{
	m_dRadius = sphere.m_dRadius;
	this->CalcSA();
	this->CalcVol();
	m_channel = sphere.m_channel;
	m_bHasConnection = sphere.m_bHasConnection;
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
		std::cout << "Cannot delete Shape.  Shape not found" << std::endl;
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
	std::cout << "What would you like the radius to be? (in mm)" << std::endl;
	std::cout << "ex: 0.125" << std::endl;

	if (isSpeech)
	{
		do
		{
			dRadius = Speech::RetrieveDouble();

			if (dRadius <= 0.0)
			{
				std::cout << "Invalid parameter.  Must be greater than zero.  Please try again." << std::endl;
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
		std::cout << "Please input a value that is not 0" << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		dRadius = std::stod(strInput);
	}

	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(dRadius);
	m_shapeVec.push_back(sphere);
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void Sphere::PrintSolids()
{
	int count = 1;

	for (auto sphere : m_shapeVec)
	{
		std::cout << count << ") " << sphere->GetName() << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
		++count;
	}
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
	int strMoveFrom = 0;
	int strMoveTo = 0;
	int sphereVecSize = static_cast<int>(Sphere::m_shapeVec.size());
	auto shapeVecItr_To = Sphere::m_shapeVec.begin();
	auto shapeVecItr_From = Sphere::m_shapeVec.begin();
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' while allowing for whitespace

	Sphere::PrintSolids();
	std::cout << std::endl;

	//------Get Sphere selections from user----------------
	do
	{
		//moveFrom Sphere:
		do
		{
			strMoveFrom = Utility::RetrieveVecInput(acceptableInputExpr, sphereVecSize);

			if (strMoveFrom == -1) return false; // user elected to go back to main menu

			if ((strMoveFrom < 1) || (strMoveFrom > sphereVecSize))
			{
				std::cout << "Invalid entry.  Please try again." << std::endl;
			}

		} while ((strMoveFrom < 1) || (strMoveFrom > sphereVecSize));

		shapeVecItr_From = std::next(shapeVecItr_From, (strMoveFrom - 1));

		//moveTo Sphere:
		do
		{
			strMoveTo = Utility::RetrieveVecInput(acceptableInputExpr, sphereVecSize);

			if (strMoveTo == -1) return false; // user elected to go back to main menu

			if ((strMoveTo < 1) || (strMoveTo > sphereVecSize))
			{
				std::cout << "Invalid entry.  Please try again." << std::endl;
			}

		} while ((strMoveTo < 1) || (strMoveTo > sphereVecSize));

		shapeVecItr_To = std::next(shapeVecItr_To, (strMoveTo - 1));

		if (strMoveFrom == strMoveTo) //check if trying to move to the same shape
		{
			std::cout << "You cannot move from and to the same Sphere.  Please try again." << std::endl;
		}

	} while (strMoveFrom == strMoveTo);

	// It's OK to now move From into To
	**shapeVecItr_To = **shapeVecItr_From; // calls move constructor
	return true;
}