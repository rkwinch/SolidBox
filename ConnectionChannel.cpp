#include "ConnectionChannel.h"
#include "Shape.h"
#include "SquarePlane.h"
#include <iostream>
#include <set>
#include <utility>
#include "Solidbox.h"


//std::pair<std::set<int>::iterator, bool> ret;
//std::set<SolidBox> CubeSet;
//std::set<SquarePlane> PlaneSet;
//std::set<std::set<SquarePlane>> PlanesOfCubes;

//constructor
ConnectionChannel::ConnectionChannel()
{
	name = "ConnectionChannel1"; //TAKE OUT LATER!!!
}

//adding a plane to the connection
//template <class T>
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
