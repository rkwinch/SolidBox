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


IMPLEMENT_SERIAL(SolidBox, CObject, 0)

int SolidBox::nameIDCounter = 1;
std::set<std::string> SolidBox::cubeNames;
const int SolidBox::planesPerSolidBox = 6;
std::map <std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>> SolidBox::cubeAndPlanesMap;
std::map<std::string, std::shared_ptr<SolidBox>> SolidBox::cubeNameAndCubeMap;

SolidBox::SolidBox() : channel(std::make_shared<ConnectionChannel>(std::shared_ptr<SolidBox>(this)))
{

}
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
		ar << hasConnection << sideLength << (int)planesPerSolidBox;
		//ex: ar << empID << empName << age;
	}

	else
	{
		int(planesPerSolidBox);
		ar >> hasConnection >> sideLength >> planesPerSolidBox;
		//ex: ar >> empID >> empName >> age;
	}

}


bool SolidBox::operator<(const SolidBox &cube) const
{
	return (this->name < cube.name);
}

bool SolidBox::operator==(const SolidBox &cube) const
{
	return (this->name == cube.name);
}

SolidBox& SolidBox::operator=(SolidBox &cube)
{
	this->sideLength = cube.sideLength;
	*(this->channel) = *(cube.channel);
	this->hasConnection = cube.hasConnection;

	//**deleting items on right side of = operator**

	//erasing string cube name
	SolidBox::cubeNames.erase(cube.name);

	//erasing string channel name
	ConnectionChannel::channelNames.erase(cube.channel->name);
	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr;
	cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(cube.name);

	//deleting string of cube name 
	std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr;
	cubeAndPlanesItr = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr->second);

	//erasing planes
	for (auto plane : cubeAndPlanesItr->second)
	{
		SquarePlane::planeNames.erase(plane->name);
	}

	//erasing cube
	SolidBox::cubeAndPlanesMap.erase(cubeAndPlanesItr->first);

	//erasing string cube name
	SolidBox::cubeNameAndCubeMap.erase(cube.name);
	//**end of deleting items from right of =**

	return *this;
}

SolidBox::~SolidBox()
{

}

void SolidBox::AddCubeNameAndCubeToMap(std::shared_ptr<SolidBox> cube)
{
	cubeNameAndCubeMap.insert(std::pair<std::string, std::shared_ptr<SolidBox>>(cube->GetShapeName(), cube));
}
void SolidBox::AddCubeAndPlanesToMap(std::shared_ptr<SolidBox> cube)
{
	cubeAndPlanesMap.insert(std::pair<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>(cube, cube->GetConnChannel()->GetPlaneSet()));
}

SolidBox::SolidBox(double sideLength) : channel(std::make_shared<ConnectionChannel>(std::shared_ptr<SolidBox>(this)))
{
	hasConnection = false; // no channel connection yet

						   //giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
						   //will verify by putting the name into a set and check if it properly inserts.
	name = Utility::CreateUniqueName("cube", cubeNames, nameIDCounter);
	cubeNames.insert(name);
	std::set<std::shared_ptr<SquarePlane>> squarePlaneSet;

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::planesPerSolidBox; i++)
	{
		std::shared_ptr<SquarePlane> plane = std::make_shared<SquarePlane>(sideLength, channel);
		squarePlaneSet.insert(plane);
	}

	channel->Connect(squarePlaneSet);

	//if all planes inserted correctly, then a proper ChannelConnection has been made
	this->sideLength = sideLength;
	hasConnection = true;
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

std::shared_ptr<ConnectionChannel> SolidBox::GetConnChannel()
{
	return channel;
}

bool SolidBox::GetHasConnection()
{
	return hasConnection;
}

