#pragma once

#include "SquarePlane.h"
#include "Shape.h"
#include <string>
#include <set>
#include "ConnectionChannel.h"
#include <map>

class SolidBox : public Shape {
	
public:
	SolidBox(double sideLength);
	~SolidBox();
	std::string GetShapeName() override;
	static void PrintCubeSet();
	double SolidBox::GetSideLength();
	ConnectionChannel GetConnChannel();
	bool operator<(const SolidBox &cube) const;
	bool operator==(const SolidBox &cube) const;
	SolidBox& operator=(const SolidBox &plane);
	static std::map <const SolidBox&, const std::set<SquarePlane*>&> cubeAndPlanesMap;
	static void SolidBox::AddCubeToMap(SolidBox &cube);


private:
	double sideLength;
	ConnectionChannel channel;
	bool hasConnection; // flag for checking if the SolidBox has a connection
	std::string name;
	static int nameIDCounter; // used for naming unique cubes
	static std::set<std::string> cubeNames; // helps enforce uniqueness of cubes created
	static const int planesPerSolidBox;
	static std::set<SolidBox> cubes;
	
};

