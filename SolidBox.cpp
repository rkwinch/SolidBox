#include "SolidBox.h"
#include "Shape.h"
#include <string>
#include "ConnectionChannel.h"
#include <set>
#include <iostream>
#include <map>
#include "SquarePlane.h"
#include "Channel.h"
#include "Utility.h"
#include "afx.h"
#include <memory>
#include <algorithm>
#include <vector>


IMPLEMENT_SERIAL(SolidBox, CObject, 0)

int SolidBox::nameIDCounter = 1;
std::set<std::string> SolidBox::cubeNames;
const int SolidBox::planesPerSolidBox = 6;
std::vector<std::shared_ptr<SolidBox>> SolidBox::cubeVec;


void SolidBox::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
	std::cout << "in serializing fxn" << std::endl;
	//double sideLength;
	//std::shared_ptr<ConnectionChannel> channel;
	//bool hasConnection; // flag for checking if the SolidBox has a connection
	//std::string name;
	//static int nameIDCounter; // used for naming unique cubes
	//static std::set<std::string> cubeNames; // helps enforce uniqueness of cubes created
	//static const int planesPerSolidBox;

	if (ar.IsStoring())
	{
		ar << bHasConnection << sideLength << (int)planesPerSolidBox;
		//ex: ar << empID << empName << age;
	}

	else
	{
		int(planesPerSolidBox);
		ar >> bHasConnection >> sideLength >> planesPerSolidBox;
		//ex: ar >> empID >> empName >> age;
	}

}

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
	name = Utility::CreateUniqueName("cube", cubeNames, nameIDCounter);
	cubeNames.insert(name);
	std::set<std::shared_ptr<SquarePlane>> squarePlaneSet;

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::planesPerSolidBox; ++i)
	{
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
	ConnectionChannel channel = other.channel;
	bHasConnection = other.bHasConnection; // flag for checking if the SolidBox has a connection
	
}

bool SolidBox::operator<(const SolidBox &cube) const
{
	return (this->name < cube.name);
}

bool SolidBox::operator==(const SolidBox &cube) const
{
	return (this->name == cube.name);
}

// since it was requested that old object be discarded afterwards, this is
// more like a move.  That is why this is not const. '=' is a move.
SolidBox& SolidBox::operator=(SolidBox &cube)
{
	sideLength = cube.sideLength;
	channel = cube.channel;
	bHasConnection = cube.bHasConnection;

	//**deleting items on right side of = operator**
	//erasing string cube name
	cubeNames.erase(cube.name);

	//erasing string channel name
	ConnectionChannel::channelNames.erase(cube.channel.name);

	//erasing cube
	std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr = cubeVec.begin();

	                                                       // [&] is take by reference, arg type is shared ptr of solidbox (box), return type is bool, 
	                                                       // predicate is check if the SolidBoxes are equivalent (same name by == operator)
	cubeVecItr = std::find_if(cubeVec.begin(), cubeVec.end(), [&](std::shared_ptr<SolidBox> box)->bool {return *box == cube; });
	cubeVec.erase(cubeVecItr);
	//**end of deleting items from right of =**

	return *this;
}

std::set<std::string>* SolidBox::GetCubeNames()
{
	return &cubeNames;
}

double SolidBox::GetSideLength()
{
	return sideLength;
}

std::string SolidBox::GetShapeName()
{
	return name;
}

ConnectionChannel* SolidBox::GetConnChannel()
{
	return &channel;
}

bool SolidBox::GetHasConnection()
{
	return bHasConnection;
}

