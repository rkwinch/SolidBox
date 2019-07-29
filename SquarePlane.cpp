#include <afx.h>
#include <iostream>
#include <vector>
#include "SquarePlane.h"
#include "Utility.h"
#include "ConnectionChannel.h"

class SolidBox;

IMPLEMENT_SERIAL(SquarePlane, CObject, 0)

int SquarePlane::nameIDCounter = 1;

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

// default constructor
SquarePlane::SquarePlane()
{
	// only here for CObject
}

// parameterized constructor
SquarePlane::SquarePlane(double sideLength, ConnectionChannel<SolidBox>* channel)
{
	name = "";
	name = Utility::CreateUniqueName("plane", nameIDCounter);
	height = sideLength;
	length = sideLength;
	numOfEdges = 4;
	this->channel = channel;
}

//defining the == operator for SquarePlane to be based on comparisons of the name of the plane
bool SquarePlane::operator==(const SquarePlane &plane) const
{
	return (this->name == plane.name);
}

//defining the = operator for SquarePlane to deep copy plane
SquarePlane& SquarePlane::operator=(const SquarePlane &plane)
{
	// don't change name here
	height = plane.height;
	length = plane.length;
	numOfEdges = plane.numOfEdges;
	return *this;
}

//defining < operator for SquarePlane to be based on comparisons of the name of the plane
bool SquarePlane::operator<(const SquarePlane &plane) const
{
	return (this->name < plane.name);
}

double SquarePlane::GetSqPlaneLength()
{
	return length;
}

double SquarePlane::GetSqPlaneHeight()
{
	return height;
}

ConnectionChannel<SolidBox>* SquarePlane::GetConnChannel()
{
	return channel;
}

int SquarePlane::GetNumOfEdges()
{
	return numOfEdges;
}

std::string SquarePlane::GetSqPlaneName()
{
	return name;
}

void SquarePlane::SetName(std::string name)
{
	this->name = name;
}

void SquarePlane::SetLength(double length)
{
	this->length = length;
}

void SquarePlane::SetHeight(double height)
{
	this->height = height;
}

void SquarePlane::SetNumOfEdges(int numOfEdges)
{
	this->numOfEdges = numOfEdges;
}
