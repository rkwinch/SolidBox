#include "SolidBox.h"
#include "Shape.h"
#include <string>
#include "ConnectionChannel.h"
#include <set>
#include <iostream>
#include <map>
#include "SquarePlane.h"


int SolidBox::nameIDCounter = 1;
std::set<SolidBox> cubeSet; // will change for mapping option later
std::set<std::string> SolidBox::cubeNames;
const int SolidBox::planesPerSolidBox = 6;
std::map <const SolidBox&, const std::set<SquarePlane*>&> cubeAndPlanesMap;

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

void SolidBox::AddCubeToMap(SolidBox &cube)
{
	std::pair<std::map<char, int>::iterator, bool> ret;
	//ret = cubeAndPlanesMap.insert(std::pair<const SolidBox&, const std::set<SquarePlane*>&>(cube, (cube.GetConnChannel).GetPlaneSet()));
	/*if (ret.second == false) {
		std::cout << (ret.first->first).GetShapeName() << "  already in map" << std::endl;*/
	//std::set<SquarePlane*>::iterator itr = ((cube.GetConnChannel).GetPlaneSet()).begin();
	/*for (auto i : (cube.GetConnChannel).GetPlaneSet())
	{
		std::cout << i.GetSqPlaneName() << std::endl;
	}*/
	//std::cout << (cube.GetConnChannel).GetPlaneSet()
	std::cout << cube.GetConnChannel().GetConnName();

		/*std::set<SquarePlane*>::iterator it=(ret.first->second).begin();
		std::cout << " with a value of " << (ret.first->second).height << '\n';*/
	
}

SolidBox::SolidBox(double sideLength)
{
	hasConnection = false; // no channel connection yet

	//giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
	//will verify by putting the name into a set and check if it properly inserts.
	name = "cube" + std::to_string(nameIDCounter++);
	std::set<std::string>::iterator cubeNameItr = cubeNames.find(name);
	try //if in set, naming collision and don't want to construct object
	{
		if (cubeNameItr != cubeNames.end())
		{
			throw std::exception();
		}
	}
	catch (std::exception e)
	{
		std::cout << "Exception:  SolidBox naming collision" << std::endl;
	}
	
	cubeNames.insert(name);
	std::string planeName = "";

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::planesPerSolidBox; i++)
	{
		SquarePlane* plane = new SquarePlane(sideLength);
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


double SolidBox::GetSideLength()
{
	return sideLength;
}

std::string SolidBox::GetShapeName()
{
	return name;
}

//static function to print SolidBoxes made so far
void SolidBox::PrintCubeSet()
{
	for (auto setElement : SolidBox::cubeNames)
	{
		std::cout << setElement << std::endl;
	}
}

ConnectionChannel SolidBox::GetConnChannel()
{
	return channel;
}
