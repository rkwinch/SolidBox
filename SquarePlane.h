#pragma once

#include <string>
#include "Shape.h"
#include "ConnectionChannel.h"
#include <afx.h>
#include <memory>

//A square plane is a plane that belongs to a solid square where a solid square
//contains 6 faces (planes).  Each plane is a 2D square that has 4 edges that are
//90' with a height and a length.  Additionally, the height = length for a square plane.

class SquarePlane : public CObject {

	friend class Utility;
	friend class SolidBox;

public:
	DECLARE_SERIAL(SquarePlane)
	void Serialize(CArchive& ar);

	static std::set<std::string> planeNames;
	static int nameIDCounter;
	SquarePlane();
	SquarePlane(double sideLength, ConnectionChannel* channel);
	bool operator==(const SquarePlane& plane) const;
	SquarePlane& operator=(const SquarePlane& plane);
	bool operator<(const SquarePlane &plane) const;
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

