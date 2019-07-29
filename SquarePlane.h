#pragma once
#include "stdafx.h"
#include <string>


//A square plane is a plane that belongs to a solid square where a solid square
//contains 6 faces (planes).  Each plane is a 2D square that has 4 edges that are
//90' with a height and a length.  Additionally, the height = length for a square plane.
class SquarePlane {
public: 
	SquarePlane(int height, int length);
	
private:
	int height;
	int length;
	int numOfEdges;
	std::string name;


};