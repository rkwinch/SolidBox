#include "SquarePlane.h"
#include "ConnectionChannel.h"
#include <afx.h>
#include "Utility.h"

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
		//ar << empID << empName << age;
	}

	else
	{
		//ar >> empID >> empName >> age;
	}

}

SquarePlane::SquarePlane(double sideLength, std::shared_ptr<ConnectionChannel> channel)
{
	name = "";
	name = Utility::CreateUniqueName("plane", planeNames, nameIDCounter);
	planeNames.insert(name);
	this->height = sideLength;
	this->length = sideLength;
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
	this->height = plane.height;
	this->length = plane.length;
	this->numOfEdges = plane.numOfEdges;
	*(this->channel) = *(plane.channel);
	//code to delete old one
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

std::shared_ptr<ConnectionChannel> SquarePlane::GetConnChannel()
{
	return channel;
}

int SquarePlane::GetNumOfEdges()
{
	return numOfEdges;
}