#include "SquarePlane.h"

SquarePlane::SquarePlane(int sideLength)
{
	this->height = length;
	this->length = length;
	numOfEdges = 4;
	name = "";

}

//defining < operator for SquarePlane to be based on comparisons of the name of the plane
bool SquarePlane::operator<(const SquarePlane& plane) const
{
	return (this->name < plane.name);
}

//defining the == operator for SquarePlane to be based on comparisons of the name of the plane
bool SquarePlane::operator==(const SquarePlane& plane) const
{
	return (this->name == plane.name);
}

//defining the = operator for SquarePlane to deep copy plane
SquarePlane& SquarePlane::operator=(const SquarePlane& plane) 
{
	this->height = plane.height;
	this->length = plane.length;
	this->numOfEdges = plane.numOfEdges;
	this->name = plane.name;
	return *this;
}
