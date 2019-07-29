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
	
}


void ConnectionChannel::Connect()
{
}

void ConnectionChannel::Disconnect()
{
}

//cleans up memory when disconnecting a channel
void ConnectionChannel::Cleanup()
{
}

//destructor
ConnectionChannel::~ConnectionChannel()
{
}

//adding a plane to the connection
void ConnectionChannel::AddPlane(SquarePlane* plane)
{
	PlaneSet.insert(plane);
}

std::set<SquarePlane*> ConnectionChannel::GetPlaneSet()
{
	return PlaneSet;
}



