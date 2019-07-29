#include "ConnectionChannel.h"
#include "Shape.h"
#include "SquarePlane.h"
#include <iostream>
#include <set>
#include "SolidBox.h"
#include "Utility.h"
#include <afx.h>
#include <memory>

IMPLEMENT_SERIAL(ConnectionChannel, CObject, 0)
int ConnectionChannel::nameIDCounter = 1;
std::set<std::string> ConnectionChannel::channelNames;

void ConnectionChannel::Serialize(CArchive& ar) {
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

ConnectionChannel::ConnectionChannel()
{
	// only here because of CObject
}

//destructor
ConnectionChannel::~ConnectionChannel()
{
	// don't need to do anything here since memory is handled via smart pointers
}

// parameterized constructor
ConnectionChannel::ConnectionChannel(SolidBox* cube)
{
	name = Utility::CreateUniqueName("connectionChannel", channelNames, nameIDCounter);
	channelNames.insert(name);
	this->cube = cube; // want the cube member here to have same address of cube
					   // it is constructed from 
	// **will get planeSet as a SolidBox is being constructed**
}

//copy constructor
ConnectionChannel::ConnectionChannel(const ConnectionChannel& channel)
{
	// new name for copy constructor was requested (will not copy current name)
	// (will probably not want to call copy constructor.  pass by reference instead)
	std::string name = Utility::CreateUniqueName("cube", channelNames, nameIDCounter);
	channelNames.insert(name);

	// allocating new memory for the copy using the length (same as height for SquarePlane
	// implementation) of a SquarePlane* in the set of channel
	for (auto planePtr : planeSet)
	{
		std::shared_ptr<SquarePlane> copyPlanePtr = std::make_shared<SquarePlane>(planePtr->GetSqPlaneHeight(), this);
		this->planeSet.insert(copyPlanePtr);
	}

	//setting cube ptr
	cube = channel.cube;
}


ConnectionChannel& ConnectionChannel::operator=(ConnectionChannel &channel)
{
	//don't change name
	std::set<std::shared_ptr<SquarePlane>>::iterator channelPlaneSetItr = channel.planeSet.begin();
	for (auto plane : planeSet)
	{
		*plane = **(channelPlaneSetItr);
		++channelPlaneSetItr;
	}

	return *this;
}

bool ConnectionChannel::operator==(const ConnectionChannel& channel) const
{
	return (name == channel.name);
}

bool ConnectionChannel::operator<(const ConnectionChannel& channel) const
{
	return (name < channel.name);
}

//adding a plane to the connection
void ConnectionChannel::Connect(std::set<std::shared_ptr<SquarePlane>> planeSet)
{
	for (auto plane : planeSet)
	{
		this->planeSet.insert(plane);
	}
}

void ConnectionChannel::Disconnect(std::set<std::shared_ptr<SquarePlane>> planeSet)
{
	for (auto plane : planeSet)
	{
		plane = nullptr;
	}
}

//cleans up memory when disconnecting a channel
void ConnectionChannel::Cleanup(SquarePlane* plane)
{
	//not really needed since using shared_ptr for SquarePlanes
}



std::set<std::shared_ptr<SquarePlane>> ConnectionChannel::GetPlaneSet()
{
	return planeSet;
}

std::string ConnectionChannel::GetConnName()
{
	return name;
}

std::set<std::string>* ConnectionChannel::GetChannelNames()
{
	return &channelNames;
}

SolidBox* ConnectionChannel::GetSolidBox()
{
	return cube;
}