#include "SolidBox.h"
#include "Shape.h"
#include <string>
#include "ConnectionChannel.h"
#include <set>
#include <iostream>
#include "SquarePlane.h"


int SolidBox::nameIDCounter = 1;
std::set<std::string>SolidBox::cubeNames;
const int SolidBox::planesPerSolidBox = 6;



SolidBox::SolidBox(double sideLength)
{
	hasConnection = false; // no channel connection yet

	//giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
	//will verify by putting the name into a set and check if it properly inserts.
	name = "cube" + std::to_string(SolidBox::nameIDCounter++);
	SolidBox::cubeNames.insert(name);
	
	std::string planeName = "";

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::planesPerSolidBox; i++)
	{
		SquarePlane* plane = new SquarePlane(sideLength);
		planeName = "plane" + std::to_string(i + 1);
		(*plane).SetSqPlaneName(planeName);
		channel.AddPlane(plane);
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
