#include <string>
#include <set>
#include <iostream>
#include <map>
#include <fstream>
#include <memory>
#include <algorithm>
#include <vector>
#include "Utility.h"
#include "SolidBox.h"
#include "ConnectionChannel.h"

int SolidBox::nameIDCounter = 0;
const int SolidBox::planesPerSolidBox = 6;
std::vector<std::shared_ptr<SolidBox>> SolidBox::cubeVec;

// default constructor 
SolidBox::SolidBox() : channel(this)
{
	// only here for CObject
}

// destructor
SolidBox::~SolidBox()
{
	// don't need to do anything here since memory is handled via smart pointers
}

// parameterized constructor
SolidBox::SolidBox(double sideLength) : channel(this)
{
	bHasConnection = false; // no channel connection yet

							//giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
							//will verify by putting the name into a set and check if it properly inserts.
	name = Utility::CreateUniqueName("cube", nameIDCounter);
	std::set<std::shared_ptr<SquarePlane>> squarePlaneSet;

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::planesPerSolidBox; ++i)
	{
		std::cout << "about to make a plane with sidelength of: " << sideLength << std::endl;
		std::shared_ptr<SquarePlane> plane = std::make_shared<SquarePlane>(sideLength, &channel);
		squarePlaneSet.insert(plane);
	}

	channel.Connect(squarePlaneSet);

	//if all planes inserted correctly, then a proper ChannelConnection has been made
	this->sideLength = sideLength;
	bHasConnection = true;
}

// copy constructor
SolidBox::SolidBox(SolidBox& other)
{
	// don't change name
	sideLength = other.sideLength;
	ConnectionChannel<SolidBox> channel = other.channel;
	bHasConnection = other.bHasConnection; // flag for checking if the SolidBox has a connection

}

// since it was requested that old object be discarded afterwards, this is
// more like a move.  That is why this is not const. '=' is a move.
SolidBox& SolidBox::operator=(SolidBox &cube)
{
	sideLength = cube.sideLength;
	channel = cube.channel;
	bHasConnection = cube.bHasConnection;
	cube.Delete(); //**deleting items on right side of = operator**
	return *this;
}

double SolidBox::GetSideLength()
{
	return sideLength;
}



int SolidBox::GetPlnsPerSolidBx()
{
	return planesPerSolidBox;
}

void SolidBox::Delete()
{
	std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr = cubeVec.begin();
	// [&] is take by reference, arg type is shared ptr of solidbox (box), return type is bool, 
	// predicate is check if the SolidBoxes are equivalent (same name by == operator)
	cubeVecItr = std::find_if(cubeVec.begin(), cubeVec.end(), [&](std::shared_ptr<SolidBox> box)->bool {return *box == *this; });
	if (cubeVecItr == cubeVec.end())
	{
		std::cout << "Cannot delete solid.  Solid not found" << std::endl;
		return;
	}
	channel.Disconnect(); // setting planes in planeSet to null
	SolidBox::cubeVec.erase(cubeVecItr); // removing item from vector
}

void SolidBox::LoadSolidBox(std::vector<std::string>::iterator &itr, const int &vecSize)
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
		for (auto planePtr : box->GetConnChannel()->GetPlaneSet())
		{
			stName = (*itr);
			itr++;
			dHeight = stod(*itr);
			itr++;
			dLength = stod(*itr);
			itr++;
			nNumOfEdges = stoi(*itr);
			itr++; std::cout << "in loadsolidbox plane. last should be a ':' " << (*itr) << std::endl;
			planePtr->SetName(stName);
			stName = ""; // resetting name
			planePtr->SetHeight(dHeight);
			planePtr->SetLength(dLength);
			planePtr->SetNumOfEdges(nNumOfEdges);
		}
		itr++; // skipping ":" delimiter
		SolidBox::cubeVec.push_back(box); // solid box object is completed now.
	}
}

void SolidBox::SaveASolidBox(std::ofstream &outFile)
{
	outFile << name << ";";
	outFile << sideLength << ";" << bHasConnection << ";";
	channel.SaveAConnectionChannel(outFile); // takes care of channel member and its associated planes
	outFile << "\n";
}