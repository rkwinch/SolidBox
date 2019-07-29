#include "Sphere.h"
#include "Shape.h"
#include <string>
#include "ConnectionChannel.h"
#include <set>
#include <iostream>
#include "Channel.h"
#include "Utility.h"
#include "afx.h"
#include <memory>
#include <algorithm>
#include <vector>

IMPLEMENT_SERIAL(Sphere, CObject, 0)

int Sphere::nameIDCounter = 1;
std::vector<std::shared_ptr<Sphere>> Sphere::sphereVec;

void Sphere::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
	std::cout << "in serializing fxn" << std::endl;

	if (ar.IsStoring())
	{
		ar << bHasConnection << radius;
	}

	else
	{
		ar >> bHasConnection >> radius;
		//ex: ar >> empID >> empName >> age;
	}
}

std::string Sphere::GetShapeName()
{
	return name;
}

double Sphere::GetRadius()
{
	return radius;
}

// default constructor 
Sphere::Sphere() : channel(this)
{
	// only here for CObject
}

// destructor
Sphere::~Sphere()
{
	// don't need to do anything here since memory is handled via smart pointers
}

// parameterized constructor
Sphere::Sphere(double radius) : channel(this)
{
	bHasConnection = false; // no channel connection yet

							//giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
							//will verify by putting the name into a set and check if it properly inserts.
	name = Utility::CreateUniqueName("sphere", nameIDCounter);
	std::set<std::shared_ptr<Sphere>> spherePlaneSet;

	channel.Connect(spherePlaneSet);

	//if all planes inserted correctly, then a proper ChannelConnection has been made
	this->radius = radius;
	bHasConnection = true;
}

// copy constructor
Sphere::Sphere(Sphere& other)
{
	// don't change name
	radius = other.radius;
	ConnectionChannel<Sphere> channel = other.channel;
	bHasConnection = other.bHasConnection; // flag for checking if the SolidBox has a connection

}

bool Sphere::operator<(const Sphere &sphere) const
{
	return (this->name < sphere.name);
}

bool Sphere::operator==(const Sphere &sphere) const
{
	return (this->name == sphere.name);
}

// since it was requested that old object be discarded afterwards, this is
// more like a move.  That is why this is not const. '=' is a move.
Sphere& Sphere::operator=(Sphere &sphere)
{
	radius = sphere.radius;
	channel = sphere.channel;
	bHasConnection = sphere.bHasConnection;
	sphere.Delete(); //**deleting items on right side of = operator**
	return *this;
}

ConnectionChannel<Sphere>* Sphere::GetConnChannel()
{
	return &channel;
}

bool Sphere::GetHasConnection()
{
	return bHasConnection;
}

void Sphere::SetName(std::string name)
{
	this->name = name;
}

void Sphere::Delete()
{
	std::vector<std::shared_ptr<Sphere>>::iterator sphereVecItr = sphereVec.begin();
	// [&] is take by reference, arg type is shared ptr of solidbox (box), return type is bool, 
	// predicate is check if the SolidBoxes are equivalent (same name by == operator)
	sphereVecItr = std::find_if(sphereVec.begin(), sphereVec.end(), [&](std::shared_ptr<Sphere> sphere)->bool {return *sphere == *this; });
	if (sphereVecItr == sphereVec.end())
	{
		std::cout << "Cannot delete solid.  Solid not found" << std::endl;
		return;
	}
	channel.Disconnect(channel.planeSet); // setting planes in planeSet to null
	Sphere::sphereVec.erase(sphereVecItr); // removing item from vector
}

