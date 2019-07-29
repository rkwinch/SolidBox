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
//#include "afx.h"

int SolidBox::nameIDCounter = 1;
std::set<SolidBox> cubeSet; // will change for mapping option later
std::set<std::string> SolidBox::cubeNames;
const int SolidBox::planesPerSolidBox = 6;
std::map <SolidBox*, std::set<SquarePlane*>> SolidBox::cubeAndPlanesMap;
std::map<std::string, double> SolidBox::cubeAndSideLengthMap;
std::map<std::string, SolidBox*> SolidBox::cubeNameAndCubeMap;

//IMPLEMENT_SERIAL(SolidBox, CObject, 1)

//void SolidBox::Serialize(CArchive& ar) {
//	CObject::Serialize(ar);
//
//	//if (ar.IsStoring())
//		//ar << empID << empName << age;
//	//else
//		//ar >> empID >> empName >> age;
//}


bool SolidBox::operator<(const SolidBox &cube) const
{
	return (this->name < cube.name);
}

bool SolidBox::operator==(const SolidBox &cube) const
{
	return (this->name == cube.name);
}

SolidBox& SolidBox::operator=(const SolidBox &cube)
{
	//dont want to change the name
	this->sideLength = cube.sideLength;
	this->channel = cube.channel;
	this->hasConnection = cube.hasConnection;
	return *this;
}

SolidBox::~SolidBox()
{

}

void SolidBox::AddCubeNameAndCubeToMap(SolidBox* cube)
{
	cubeNameAndCubeMap.insert(std::pair<std::string, SolidBox*>(cube->GetShapeName(), cube));
}
void SolidBox::AddCubeAndPlanesToMap(SolidBox* cube)
{
	cubeAndPlanesMap.insert(std::pair<SolidBox*, std::set<SquarePlane*>>(cube, cube->GetConnChannel()->GetPlaneSet()));
}

SolidBox::SolidBox(double sideLength) : channel(this)
{
	hasConnection = false; // no channel connection yet

						   //giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
						   //will verify by putting the name into a set and check if it properly inserts.
	name = Utility::CreateUniqueName("cube", cubeNames, nameIDCounter);
	cubeAndSideLengthMap.insert(std::pair<std::string, double>(name, sideLength));
	cubeNames.insert(name);
	std::string planeName = "";

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::planesPerSolidBox; i++)
	{
		SquarePlane* plane = new SquarePlane(sideLength, &channel);
		planeName = "plane" + std::to_string(i + 1);
		plane->SetSqPlaneName(planeName);
		channel.Connect(plane);
	}

	//if all planes inserted correctly, then a proper ChannelConnection has been made
	if ((channel.GetPlaneSet()).size() == 6)
	{
		this->sideLength = sideLength;
		hasConnection = true;
	}
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
	return hasConnection;
}

