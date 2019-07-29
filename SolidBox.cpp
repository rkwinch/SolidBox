#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <vector>
#include "SolidBox.h"
#include "RectPlane.h"
#include "ConnectionChannel.h"
#include "Utility.h"

// default constructor 
SolidBox::SolidBox() : m_channel(this)
{
	// only here for CObject
}

// destructor
SolidBox::~SolidBox()
{
	// don't need to do anything here since memory is handled via smart pointers
}

// parameterized constructor
SolidBox::SolidBox(double sideLength) : m_channel(this)
{
	//giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
	//will verify by putting the name into a set and check if it properly inserts.
	m_stName = Shape::CreateUniqueName("cube", m_nNameIDCounter);
	std::set<std::shared_ptr<RectPlane<SolidBox>>> RectPlaneSet;

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::m_nNumOfSurfaces; ++i)
	{
		std::cout << "about to make a plane with sidelength of: " << sideLength << std::endl;
		std::shared_ptr<RectPlane<SolidBox>> plane = std::make_shared<RectPlane<SolidBox>>(sideLength, sideLength, &m_channel);
		RectPlaneSet.insert(plane);
	}

	m_channel.Connect(RectPlaneSet);

	//if all planes inserted correctly, then a proper ChannelConnection has been made
	this->m_dSideLength = sideLength;
	m_bHasConnection = true;
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

double SolidBox::GetSideLength()
{
	return m_dSideLength;
}

ConnectionChannel<SolidBox, RectPlane<SolidBox>>* SolidBox::GetConnChannel()
{
	return &m_channel;
}

void SolidBox::Create()
{
	std::string strInput = "";
	double dSideLength = 0.0;
	std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
																	// with 0-1 decimals followed by a number (if present) while allowing spaces
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "What would you like the length, width, and height to be? (in mm)" << std::endl;
	std::cout << "ex: 4.5" << std::endl;
	strInput = Utility::GetAndValidateInput(acceptableInputExpr);
	dSideLength = std::stod(strInput); // converting string input into a double

	while (dSideLength == 0.0) // don't want to make a box with sideLength of 0
	{
		std::cout << "Please input a value that is not 0" << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		dSideLength = std::stod(strInput);
	}
	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(dSideLength);
	SolidBox::m_shapeVec.push_back(box);
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

std::string SolidBox::GetConnName()
{
	return m_channel.GetName();
}

void SolidBox::Delete()
{
	std::vector<std::shared_ptr<SolidBox>>::iterator shapeVecItr = m_shapeVec.begin();
	// [&] is take by reference, arg type is shared ptr of surface type (solidbox or sphere at this point), return type is bool, 
	// predicate is check if the shapes are equivalent (same name by == operator)
	shapeVecItr = std::find_if(m_shapeVec.begin(), m_shapeVec.end(), [&](std::shared_ptr<SolidBox> shape)->bool {return *shape == *this; });

	if (shapeVecItr == m_shapeVec.end())
	{
		std::cout << "Cannot delete solid.  Solid not found" << std::endl;
		return;
	}

	m_channel.Disconnect(); // setting surfaces in surfaceSet to null
	m_shapeVec.erase(shapeVecItr); // removing item from vector
}

void SolidBox::PrintSolids()
{
	int count = 1;

	for (auto cube : SolidBox::m_shapeVec)
	{
		std::cout << count << ") " << cube->m_stName << std::endl;
		++count;
	}
}

void SolidBox::Load(std::vector<std::string>::iterator &itr, const int &vecSize)
{
	std::string stName = "";
	double dLength = 0;
	bool bHasConnection = false;
	double dHeight = 0;
	int nNumOfEdges = 0;

	//getting members for solidbox(es)
	for (int ii = 0; ii < vecSize; ++ii)
	{
		stName = ""; // resetting name for boxes if there is more than one solid box
		stName = (*itr); std::cout << "in loadsolidbox should be a name: " << (*itr) << std::endl;
		itr++; std::cout << "in loadsolidbox should be a double: " << (*itr) << std::endl;
		dLength = stod(*itr);
		itr++;
		bHasConnection = static_cast<bool>(stoi(*itr));
		itr++;

		//constructing a solid box with given length and setting other params
		std::cout << "making a solidbox from load with length,hasconn,name: " << dLength << " " << bHasConnection << " " << stName << std::endl;
		std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(dLength);
		box->SetName(stName);
		stName = ""; // resetting name

		//getting connectionchannel name
		stName = (*itr);
		itr++;
		box->GetConnChannel()->SetName(stName);
		stName = ""; // resetting name

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
			itr++; std::cout << "in loadsolidbox plane. last should be a ':' " << (*itr) << std::endl;
			// for notes on why using dynamic cast here, refer to load fxn in Sphere
			auto rectPlanePtr = dynamic_cast<RectPlane<SolidBox>*>(planePtr.get());
			rectPlanePtr->SetName(stName);
			stName = ""; // resetting name
			rectPlanePtr->SetHeight(dHeight);
			rectPlanePtr->SetLength(dLength);
			rectPlanePtr->SetNumOfEdges(nNumOfEdges);
		}
		itr++; // skipping ":" delimiter
		SolidBox::m_shapeVec.push_back(box); // solid box object is completed now.
	}
}

void SolidBox::Save(std::ofstream &outFile)
{
	outFile << m_stName << ";";
	outFile << m_dSideLength << ";" << m_bHasConnection << ";";
	m_channel.Save(outFile); // takes care of channel member and its associated surfaces
	outFile << "\n";
}

std::set<std::shared_ptr<RectPlane<SolidBox>>> SolidBox::GetSurfacesCopy() override 
{
	std::set<std::shared_ptr<RectPlane<SolidBox>>> surfaceSet;
	for (auto surface : m_channel.GetSurfaceSet())
	{
		std::shared_ptr<RectPlane<SolidBox>> copy = std::make_shared<RectPlane<SolidBox>>(m_dSideLength, m_dSideLength, &m_channel);
		surfaceSet.insert(copy);
	}
	return surfaceSet;
}

int SolidBox::GetSurfaceCount()
{
	return SolidBox::m_nNumOfSurfaces;
}