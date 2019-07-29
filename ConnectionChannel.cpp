#include "ConnectionChannel.h"
#include "Shape.h"
#include "SquarePlane.h"
#include <iostream>
#include <set>
//#include <utility>
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
		//ar << empID << empName << age;
	}

	else
	{
		//ar >> empID >> empName >> age;
	}

}
ConnectionChannel::ConnectionChannel()
{

}
ConnectionChannel& ConnectionChannel::operator=(ConnectionChannel &channel)
{
	std::cout << "***********in conn channel =" << std::endl;
	//don't change name
	//std::set<std::shared_ptr<SquarePlane>> planeSet;
	std::set<std::shared_ptr<SquarePlane>>::iterator channelPlaneSetItr = channel.planeSet.begin();
	for (auto plane : planeSet)
	{
		*plane = **(channelPlaneSetItr);
		channelPlaneSetItr++;
	}
	
	//*(this->planeSet) = *(channel.planeSet);

	//*(this->cube) = *(channel.cube);
	//this->Disconnect(planeSet);
	//this->Connect(channel.planeSet);
	//channel.Disconnect(planeSet);
	//code to delete old channel
	return *this;
}

// parameterized constructor
ConnectionChannel::ConnectionChannel(std::shared_ptr<SolidBox> cube)
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
		std::shared_ptr<SquarePlane> copyPlanePtr = std::make_shared<SquarePlane>(planePtr->GetSqPlaneHeight(), std::shared_ptr<ConnectionChannel>(this));
		this->planeSet.insert(copyPlanePtr);
	}	

	//allocating new memory for a copy of the cube constructed with the same sideLength
	std::shared_ptr<SolidBox> copyCube = std::make_shared<SolidBox>(this->cube->GetSideLength());
	this->cube = copyCube;
}

//adding a plane to the connection
void ConnectionChannel::Connect(std::set<std::shared_ptr<SquarePlane>> planeSet)
{
	for (auto plane : planeSet)
	{
		this->planeSet.insert(plane);
	}
}

//template <class T>
void ConnectionChannel::Disconnect(std::set<std::shared_ptr<SquarePlane>> planeSet)
{
	for (auto plane : planeSet)
	{
		plane = nullptr;
	}
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

std::set<std::shared_ptr<SquarePlane>> ConnectionChannel::GetPlaneSet()
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

std::shared_ptr<SolidBox> ConnectionChannel::GetSolidBox()
{
	return cube;
}