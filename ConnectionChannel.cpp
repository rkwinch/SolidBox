#include "ConnectionChannel.h"
#include "Shape.h"
#include "SquarePlane.h"
#include <iostream>
#include <set>
//#include <utility>
#include "SolidBox.h"
#include "Utility.h"


int ConnectionChannel::nameIDCounter = 1;
std::set<std::string> ConnectionChannel::channelNames;

// parameterized constructor
ConnectionChannel::ConnectionChannel(SolidBox* cube)
{
    name = Utility::CreateUniqueName("connectionChannel", channelNames, nameIDCounter);
	channelNames.insert(name);

	// will get planeSet as a SolidBox is being constructed

	this->cube = cube; // want the cube member here to have same address of cube
	                   // it is constructed from
}

//copy constructor
ConnectionChannel::ConnectionChannel(const ConnectionChannel& channel)
{
	// new name for copy constructor was requested (will not copy current name)
	std::string name = Utility::CreateUniqueName("cube", channelNames, nameIDCounter);
	channelNames.insert(name);

	// allocating new memory for the copy using the length (same as height for SquarePlane
	// implementation) of a SquarePlane* in the set of channel
	for (auto planePtr : planeSet)
	{
		SquarePlane* copyPlanePtr = new SquarePlane(planePtr->GetSqPlaneHeight(), this);
		this->planeSet.insert(copyPlanePtr);
	}	

	//allocating new memory for a copy of the cube constructed with the same sideLength
	SolidBox* copyCube = new SolidBox(this->cube->GetSideLength());
	this->cube = copyCube;
}

//adding a plane to the connection
void ConnectionChannel::Connect(SquarePlane* plane)
{
	planeSet.insert(plane);
}

//template <class T>
void ConnectionChannel::Disconnect(SquarePlane* plane)
{

}

//cleans up memory when disconnecting a channel
//template <class T>
void ConnectionChannel::Cleanup(SquarePlane* plane)
{
}

//destructor
ConnectionChannel::~ConnectionChannel()
{
}

std::set<SquarePlane*> ConnectionChannel::GetPlaneSet()
{
	return planeSet;
}

std::string ConnectionChannel::GetConnName()
{
	return name;
}

bool ConnectionChannel::operator==(const ConnectionChannel& channel) const
{
	return (this->name == channel.name);
}

bool ConnectionChannel::operator<(const ConnectionChannel& channel) const
{
	return (this->name < channel.name);
}

std::set<std::string>* ConnectionChannel::GetChannelNames()
{
	return &ConnectionChannel::channelNames;
}

SolidBox* ConnectionChannel::GetSolidBox()
{
	return cube;
}