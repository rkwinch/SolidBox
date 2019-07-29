#pragma once

#include "SquarePlane.h"
#include "Shape.h"
#include "Channel.h"
#include <string>
#include <set>

class SolidBox : public Shape {
	
public:
	SolidBox(int sideLength);
	std::string GetShapeName() override;
	static void PrintCubeSet();


private:
	
	std::string name;
	static int nameIDCounter; // used for naming unique cubes
	static std::set<std::string> cubeNames; // helps enforce uniqueness of cubes created
	static const int planesPerSolidBox;
	std::vector<SquarePlane> groupOfPlanes;
};

