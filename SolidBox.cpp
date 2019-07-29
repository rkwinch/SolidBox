#include "SolidBox.h"
#include "Shape.h"
#include <string>
#include "ConnectionChannel.h"
#include <set>
#include <iostream>
#include "SquarePlane.h"


int SolidBox::nameIDCounter = 1; // why do I need to have int here?  The type is in the header
std::set<std::string>SolidBox::cubeNames;
const int SolidBox::planesPerSolidBox = 6;


SolidBox::SolidBox(int sideLength)
{
	//giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
	//will verify by putting the name into a set and check if it properly inserts.
	name = "cube" + std::to_string(SolidBox::nameIDCounter++);
	SolidBox::cubeNames.insert(name);
	
	std::cout << "in solidbox constructor" << std::endl;

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::planesPerSolidBox; i++) 
	{
		groupOfPlanes.emplace_back(SquarePlane(sideLength));
	}

	for (int ii = 0; ii < SolidBox::planesPerSolidBox; ++ii)
	{
		std::cout << "plane" << ii << "in " << name << std::endl;
		SquarePlane plane(2);
	}
	//	std::pair<std::set<SquarePlane>::iterator, bool> returnVal; // for return value for insert fxn for sets
	//	returnVal = PlaneSet.insert(plane);
	//	if (returnVal.second == false) //returnVal.second is a bool that is false if didn't insert (dup) or true if did
	//	{
	//		std::cout << "Plane already inserted.  No duplicates." << std::endl;
	//	}
	//	else
	//	{
	//		std::cout << "Plane successfully inserted!" << std::endl;
	//	}
	//}
}

//std::vector<SquarePlane*> groupOfPlanes;

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

