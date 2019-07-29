#pragma once

#include "SquarePlane.h"
#include "Shape.h"
#include <string>
#include <set>
#include "ConnectionChannel.h"

class SolidBox : public Shape {
	
public:
	SolidBox(double sideLength);
	std::string GetShapeName() override;
	static void PrintCubeSet();
	double SolidBox::GetSideLength();
	ConnectionChannel GetConnChannel();

private:
	double sideLength;
	ConnectionChannel channel;
	bool hasConnection; // flag for checking if the SolidBox has a connection
	std::string name;
	static int nameIDCounter; // used for naming unique cubes
	static std::set<std::string> cubeNames; // helps enforce uniqueness of cubes created
	static const int planesPerSolidBox;
};

