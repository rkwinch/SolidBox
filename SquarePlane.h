#pragma once

#include <string>
#include "Shape.h"
#include "ConnectionChannel.h"


//A square plane is a plane that belongs to a solid square where a solid square
//contains 6 faces (planes).  Each plane is a 2D square that has 4 edges that are
//90' with a height and a length.  Additionally, the height = length for a square plane.
class SquarePlane {
public:
	SquarePlane(double sideLength, ConnectionChannel* channel);
	bool operator==(const SquarePlane& plane) const;
	SquarePlane& operator=(const SquarePlane& plane);
	bool operator<(const SquarePlane* plane) const;
	void SetSqPlaneName(std::string name);
	std::string GetSqPlaneName();
	double GetSqPlaneLength();
	double GetSqPlaneHeight();
	ConnectionChannel* GetConnChannel();
	int GetNumOfEdges();

private:
	double height;
	double length;
	int numOfEdges;
	std::string name;
	ConnectionChannel* channel;

};
