#pragma once

#include <string>
#include <memory>
#include "ConnectionChannel.h"

//A square plane is a plane that belongs to a solid square where a solid square
//contains 6 faces (planes).  Each plane is a 2D square that has 4 edges that are
//90' with a height and a length.  Additionally, the height = length for a square plane.

class SolidBox;

class SquarePlane {

	friend class Utility;
	friend class SolidBox;

public:
	static int nameIDCounter;
	SquarePlane();
	SquarePlane(double sideLength, ConnectionChannel<SolidBox>* channel);
	bool operator==(const SquarePlane& plane) const;
	SquarePlane& operator=(const SquarePlane& plane);
	bool operator<(const SquarePlane &plane) const;
	std::string GetSqPlaneName();
	double GetSqPlaneLength();
	double GetSqPlaneHeight();
	ConnectionChannel<SolidBox>* GetConnChannel();
	int GetNumOfEdges();
	void SetName(std::string name);
	void SetLength(double length);
	void SetHeight(double height);
	void SetNumOfEdges(int numOfEdges);
	void SaveASquarePlane(std::ofstream &outFile);

private:
	double height;
	double length;
	int numOfEdges;
	std::string name;
	ConnectionChannel<SolidBox>* channel;
};