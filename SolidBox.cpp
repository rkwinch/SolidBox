#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <iomanip>
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
SolidBox::SolidBox(SolidBox& other)
{
	// don't change name
	m_dSideLength = other.m_dSideLength;
	m_channel = other.m_channel;
	m_bHasConnection = other.m_bHasConnection; // flag for checking if the SolidBox has a connection

}

// since it was requested that old object be discarded afterwards, this is
// more like a move.  That is why this is not const. '=' is a move.
SolidBox& SolidBox::operator=(SolidBox &cube)
{
	m_dSideLength = cube.m_dSideLength;
	m_channel = cube.m_channel;
	m_bHasConnection = cube.m_bHasConnection;
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
		std::cout << "Cannot delete Shape.  Shape not found" << std::endl;
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

	std::cout << "What would you like the length, width, and height to be? (in mm)" << std::endl;
	std::cout << "ex: 4.5" << std::endl;

	if (isSpeech)
	{
		do
		{
			dSideLength = Speech::RetrieveDouble(); // getting double from spoken phrase

			if (dSideLength <= 0)
			{
				std::cout << "Please say a positive, nonzero, number." << std::endl;
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
				std::cout << "Please enter a positive, nonzero, number." << std::endl;
			}
		} while (dSideLength <= 0.0);
	}
	
	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(dSideLength);
	m_shapeVec.push_back(box);
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void SolidBox::PrintSolids()
{
	int count = 1;

	for (auto cube : m_shapeVec)
	{
		std::cout << count << ") " << cube->GetName() << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
		++count;
	}
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
		std::cout << "Invalid length and height parameters.  Cannot load cube." << std::endl;
		box->Delete();
	}
}

bool SolidBox::Move()
{
	int strMoveFrom = 0;
	int strMoveTo = 0;
	int solidBoxVecSize = static_cast<int>(SolidBox::m_shapeVec.size());
	auto shapeVecItr_To = SolidBox::m_shapeVec.begin();
	auto shapeVecItr_From = SolidBox::m_shapeVec.begin();
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' while allowing for whitespace

	SolidBox::PrintSolids();
	std::cout << std::endl;

	//------Get cube selections from user----------------
	do
	{  
		//moveFrom cube:
		do
		{
			strMoveFrom = Utility::RetrieveVecInput(acceptableInputExpr, SolidBox::m_shapeVec.size());

			if (strMoveFrom == -1) return false; // user elected to go back to main menu
			
			if ((strMoveFrom < solidBoxVecSize) || (strMoveFrom > solidBoxVecSize))
			{
				std::cout << "Invalid entry.  Please try again." << std::endl;
			}

		} while ((strMoveFrom < solidBoxVecSize) || (strMoveFrom > solidBoxVecSize));
		
		shapeVecItr_From = std::next(shapeVecItr_From, (strMoveFrom - 1));

		//moveTo cube:
		do
		{
			strMoveTo = Utility::RetrieveVecInput(acceptableInputExpr, solidBoxVecSize);

			if (strMoveTo == -1) return false; // user elected to go back to main menu

			if ((strMoveTo < solidBoxVecSize) || (strMoveTo > solidBoxVecSize))
			{
				std::cout << "Invalid entry.  Please try again." << std::endl;
			}

		} while ((strMoveTo < solidBoxVecSize) || (strMoveTo > solidBoxVecSize));
		
		shapeVecItr_To = std::next(shapeVecItr_To, (strMoveTo - 1));

		if (strMoveFrom == strMoveTo) //check if trying to move to the same shape
		{
			std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
		}

	} while (strMoveFrom == strMoveTo);

	// It's OK to now move From into To
	**shapeVecItr_To = **shapeVecItr_From;
	return true;
}