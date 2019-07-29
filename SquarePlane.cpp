#include "SquarePlane.h"
#include "ConnectionChannel.h"
#include <afx.h>
#include "Utility.h"
#include <iostream>

IMPLEMENT_SERIAL(SquarePlane, CObject, 0)

std::set<std::string> SquarePlane::planeNames;
int SquarePlane::nameIDCounter = 1;

SquarePlane::SquarePlane()
{

}

void SquarePlane::Serialize(CArchive& ar) {
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		//ex: ar << empID << empName << age;
	}

	else
	{
		//ex: ar >> empID >> empName >> age;
	}

}

SquarePlane::SquarePlane(double sideLength, ConnectionChannel* channel)
{
	name = "";
	name = Utility::CreateUniqueName("plane", planeNames, nameIDCounter);
	planeNames.insert(name);
	height = sideLength;
	length = sideLength;
	numOfEdges = 4;
	this->channel = channel;
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
	height = plane.height;
	length = plane.length;
	numOfEdges = plane.numOfEdges;
	return *this;
}

//defining < operator for SquarePlane to be based on comparisons of the name of the plane
bool SquarePlane::operator<(const std::shared_ptr<SquarePlane> plane) const
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