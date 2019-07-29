#include "SquarePlane.h"
#include "ConnectionChannel.h"

SquarePlane::SquarePlane(double sideLength, ConnectionChannel* channel)
{
	this->height = sideLength;
	this->length = sideLength;
	numOfEdges = 4;
	name = "";
	this->channel = channel;
}

void SquarePlane::SetSqPlaneName(std::string name)
{
	this->name = name;
}

std::string SquarePlane::GetSqPlaneName()
{
	return name;
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

//defining < operator for SquarePlane to be based on comparisons of the name of the plane
bool SquarePlane::operator<(const SquarePlane* plane) const
{
	return (this->name < plane->name);
}

double SquarePlane::GetSqPlaneLength()
{
	return length;
}

double SquarePlane::GetSqPlaneHeight()
{
	return height;
}

ConnectionChannel* SquarePlane::GetConnChannel()
{
	return channel;
}

int SquarePlane::GetNumOfEdges()
{
	return numOfEdges;
}