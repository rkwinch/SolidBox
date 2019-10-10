#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <iomanip>
#include <sstream>
#include "SolidBox.h"
#include "RectPlane.h"
#include "ConnectionChannel.h"
#include "Utility.h"
#include "Menu.h"
#include "Speech.h"

const int SolidBox::m_nSurfaces = 1;
int SolidBox::m_nNameIDCounter = 0;
std::vector<std::shared_ptr<SolidBox>> SolidBox::m_shapeVec;

// parameterized constructor
SolidBox::SolidBox(double sideLength)
{
	//giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
	//will verify by putting the name into a set and check if it properly inserts.
	m_stName = Utility::CreateUniqueName("cube", m_nNameIDCounter);
	m_nNumOfSurfaces = 6;
	std::set<std::shared_ptr<Surface>> rectPlaneSet;

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::m_nNumOfSurfaces; ++i)
	{
		std::shared_ptr<RectPlane> plane = std::make_shared<RectPlane>(sideLength, sideLength, &m_channel);
		rectPlaneSet.insert(plane);
	}

	m_channel.Connect(rectPlaneSet);

	//if all planes inserted correctly, then a proper ChannelConnection has been made
	m_dSideLength = sideLength;
	CalcSA();
	CalcVol();
	m_bHasConnection = true;
}

// destructor
SolidBox::~SolidBox()
{
	// don't need to do anything here since memory is handled via smart pointers
}

// copy constructor
SolidBox::SolidBox(const SolidBox& other) : Shape(other)
{
	// don't change name
	if (m_stName.length() == 0)
	{
		m_stName = Utility::CreateUniqueName("cube", m_nNameIDCounter);
	}

	m_nNumOfSurfaces = other.m_nNumOfSurfaces;
	m_dSideLength = other.m_dSideLength;
	m_bHasConnection = other.m_bHasConnection;
	m_channel.SetShape(this);
	CalcSA();
	CalcVol();
}

// since it was requested that old object be discarded afterwards, this is
// more like a move.  That is why this is not const. '=' is a move.
SolidBox& SolidBox::operator=(SolidBox &cube)
{
	m_dSideLength = cube.m_dSideLength;
	m_channel = cube.m_channel;
	m_bHasConnection = cube.m_bHasConnection;
	CalcSA();
	CalcVol();
	cube.Delete(); //**deleting items on right side of = operator**
	return *this;
}

void SolidBox::Delete()
{
	std::vector<std::shared_ptr<SolidBox>>::iterator shapeVecItr = m_shapeVec.begin();
	// [&] is take by reference, arg type is shared ptr of surface type (solidbox or sphere at this point), return type is bool, 
	// predicate is check if the shapes are equivalent (same name by == operator)
	shapeVecItr = std::find_if(m_shapeVec.begin(), m_shapeVec.end(), [&](std::shared_ptr<SolidBox> shape)->bool {return *shape == *this; });

	if (shapeVecItr == m_shapeVec.end())
	{
		Utility::Display("Cannot delete Shape.  Shape not found\n");
		return;
	}

	m_channel.Disconnect(); // setting surfaces in surfaceSet to null
	m_shapeVec.erase(shapeVecItr); // removing item from vector
}

void SolidBox::CalcVol()
{
	m_dVolume = pow(m_dSideLength, 3.0);
}

void SolidBox::CalcSA()
{
	m_dSurfaceArea = 6.0 * pow(m_dSideLength, 2.0);
}

double SolidBox::GetSideLength()
{
	return m_dSideLength;
}

std::set<std::shared_ptr<Surface>> SolidBox::GetSurfacesCopy()
{
	std::set<std::shared_ptr<Surface>> surfaceSet;

	for (auto surface : m_channel.GetSurfaceSet())
	{
		auto element = dynamic_cast<RectPlane*>(surface.get());
		std::shared_ptr<RectPlane> copy = std::make_shared<RectPlane>(element->GetHeight(), element->GetLength(), &m_channel);
		surfaceSet.insert(copy); // don't do error checking for set insertion since was previously in a set
	}
	return surfaceSet;
}

std::vector<std::shared_ptr<SolidBox>> SolidBox::GetShapeVec()
{
	return m_shapeVec;
}

void SolidBox::Save(std::ofstream &outFile)
{
	outFile << m_stName << ";";
	outFile << m_dSideLength << ";" << m_bHasConnection << ";";
	m_channel.Save(outFile); // takes care of channel member and its associated surfaces
	outFile << "\n";
}

void SolidBox::Create()
{
	std::string strInput = "";
	double dSideLength = 0.0;
	std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
																	// with 0-1 decimals followed by a number (if present) while allowing spaces
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	Utility::Display("What would you like the length, width, and height to be? (in mm)\n");
	Utility::Display("ex: 4.5\n");

	if (isSpeech)
	{
		do
		{
			dSideLength = Speech::RetrieveDouble(); // getting double from spoken phrase

			if (dSideLength <= 0)
			{
				Utility::Display("Please say a positive, nonzero, number.\n");
			}

		} while (dSideLength <= 0.0);
	}
	else
	{
		do
		{
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);
			dSideLength = std::stod(strInput); // converting string input into a double

			if (dSideLength <= 0.0)
			{
				Utility::Display("Please enter a positive, nonzero, number.\n");
			}
		} while (dSideLength <= 0.0);
	}

	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(dSideLength);
	m_shapeVec.push_back(box);
	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
}

std::string SolidBox::PrintSolids()
{
	std::ostringstream stream;
	int count = 1;

	for (auto cube : m_shapeVec)
	{
		stream << count << ") " << cube->GetName() << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
		++count;
	}

	return stream.str();
}

std::string SolidBox::PrintSolids(int &counter)
{
	std::ostringstream stream;

	for (auto &cube : SolidBox::m_shapeVec)
	{
		stream << counter++ << ") " << cube->GetName() << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
	}

	return stream.str();
}

void SolidBox::Load(std::vector<std::string>::iterator &itr)
{
	std::string stName = "";
	double dLength = 0;
	bool bHasConnection = false;
	double dHeight = 0;
	int nNumOfEdges = 0;

	//getting members for solidbox
	stName = (*itr);
	itr++;
	dLength = stod(*itr);
	itr++;
	bHasConnection = static_cast<bool>(stoi(*itr));
	itr++;

	//constructing a solid box with given length and setting other params
	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(dLength);
	box->SetName(stName);

	//getting connectionchannel name
	stName = (*itr);
	itr++;
	box->GetConnChannel()->SetName(stName);

	//getting and setting members for square planes
	for (auto planePtr : box->GetConnChannel()->GetSurfaceSet())
	{
		stName = (*itr);
		itr++;
		dHeight = stod(*itr);
		itr++;
		dLength = stod(*itr);
		itr++;
		nNumOfEdges = stoi(*itr);
		itr++;
		// for notes on why using dynamic cast here, refer to load fxn in Sphere
		auto rectPlanePtr = dynamic_cast<RectPlane*>(planePtr.get());
		rectPlanePtr->SetName(stName);
		stName = ""; // resetting name
	}

	SolidBox::m_shapeVec.push_back(box); // solid box object is completed now.

	if (dHeight != dLength)
	{
		Utility::Display("Invalid length and height parameters.  Cannot load cube.\n");
		box->Delete();
	}
}

bool SolidBox::Move()
{
	int nMoveFrom = 0;
	int nMoveTo = 0;
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();
	int solidBoxVecSize = static_cast<int>(SolidBox::m_shapeVec.size());
	auto shapeVecItr_To = SolidBox::m_shapeVec.begin();
	auto shapeVecItr_From = SolidBox::m_shapeVec.begin();
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' while allowing for whitespace

	Utility::Display(SolidBox::PrintSolids());
	Utility::Display("\n");

	//------Get cube selections from user----------------
	do
	{
		//moveFrom cube:
		do
		{
			if (isSpeech)
			{
				Utility::Display("Please select the cube you are moving FROM or say \"back\" to go to the main menu.\n");
				nMoveFrom = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("Please select the cube you are moving FROM or press 'b' to go back to the main menu.\n");
				nMoveFrom = Utility::RetrieveVecInput(acceptableInputExpr, solidBoxVecSize);
			}

			if (nMoveFrom == -1) return false; // user elected to go back to main menu

			if ((nMoveFrom < 1) || (nMoveFrom > solidBoxVecSize))
			{
				Utility::Display("Invalid entry.  Please try again.\n");
			}

		} while ((nMoveFrom < 1) || (nMoveFrom > solidBoxVecSize));

		shapeVecItr_From = std::next(shapeVecItr_From, (nMoveFrom - 1));

		//moveTo cube:
		do
		{
			if (isSpeech)
			{
				Utility::Display("Please select the cube you are moving TO or say \"back\" to go to the main menu.\n");
				nMoveTo = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("Please select the cube you are moving TO or press 'b' to go back to the main menu.\n");
				nMoveTo = Utility::RetrieveVecInput(acceptableInputExpr, solidBoxVecSize);
			}

			if (nMoveTo == -1) return false; // user elected to go back to main menu

			if ((nMoveTo < 1) || (nMoveTo > solidBoxVecSize))
			{
				Utility::Display("Invalid entry.  Please try again.\n");
			}

		} while ((nMoveTo < 1) || (nMoveTo > solidBoxVecSize));

		shapeVecItr_To = std::next(shapeVecItr_To, (nMoveTo - 1));

		if (nMoveFrom == nMoveTo) //check if trying to move to the same shape
		{
			Utility::Display("You cannot move from and to the same cube.  Please try again.\n");
		}

	} while (nMoveFrom == nMoveTo);

	// It's OK to now move From into To
	**shapeVecItr_To = **shapeVecItr_From;
	return true;
}

bool SolidBox::Move(std::shared_ptr<SolidBox> cube)
{
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();
	int nMoveTo = 0;
	int counter = 1;
	std::ostringstream stream;
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' while allowing for whitespace
	int solidBoxVecSize = static_cast<int>(SolidBox::m_shapeVec.size());
	auto shapeVecItr_From = std::find(SolidBox::m_shapeVec.begin(), SolidBox::m_shapeVec.end(), cube);
	auto shapeVecItr_To = SolidBox::m_shapeVec.begin();
	
	do
	{
		//moveTo cube:
		do
		{
			if (isSpeech)
			{
				Utility::Display("\nPlease select the cube you are moving TO or say \"back\" to go to the main menu.\n\n");
				Utility::Display(SolidBox::PrintSolids());
				nMoveTo = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("\nPlease select the cube you are moving TO or press 'b' to go back to the main menu.\n\n");
				Utility::Display(SolidBox::PrintSolids());
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
			Utility::Display("You cannot move from and to the same cube.  Please try again.\n");
		}

	} while (*shapeVecItr_To == *shapeVecItr_From);

	// It's OK to now move From into To
	**shapeVecItr_To = **shapeVecItr_From;
	return true;
}

std::string SolidBox::PrintShapeInfo()
{
	std::string output = "";
	std::string strHeader = "Solid Box:";
	std::ostringstream stream;

	stream << Utility::PrintHeader(strHeader);
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Shape name:" << m_stName << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "hasConnection:" << m_bHasConnection << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Channel name:" << m_channel.GetName() << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "SideLength (mm):" << std::fixed << std::setprecision(3) << m_dSideLength << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Surface Area (mm^2):" << m_dSurfaceArea << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Volume (mm^3):" << m_dVolume << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Number of surfaces:" << m_nNumOfSurfaces << std::endl;
	stream << std::endl;
	return output = stream.str();
}

std::string SolidBox::PrintPlanesInfo()
{
	std::string output = "";
	std::ostringstream stream;

	stream << Utility::PrintHeader("Surfaces:");

	for (auto plane : m_channel.GetSurfaceSet())
	{
		auto planePtr = dynamic_cast<RectPlane*>(plane.get());
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Plane name:" << plane->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Associated shape name:" << plane->GetConnChannel()->GetShape()->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Associated channel name:" << plane->GetConnChannel()->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Number of edges:" << plane->GetNumOfEdges() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Length:" << std::fixed << std::setprecision(3) << planePtr->GetLength() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Height:" << std::fixed << std::setprecision(3) << planePtr->GetHeight() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Area:" << std::fixed << std::setprecision(3) << planePtr->GetArea() << std::endl;
		stream << std::endl;
	}

	return output = stream.str();
}