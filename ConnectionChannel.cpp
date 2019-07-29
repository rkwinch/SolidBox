#include "ConnectionChannel.h"
#include "Shape.h"
#include "SquarePlane.h"
#include <iostream>
#include <set>
#include <utility>


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
//adding set of planes to the connection
void ConnectionChannel::AddPlanes(const std::set<SquarePlane> PlaneSet)
{
	
	this->PlaneSet = PlaneSet;
}

//void ConnectionChannel::AddPlaneToSet(const SquarePlane& plane)
//{
//	std::pair<std::set<SquarePlane>::iterator, bool> returnVal; // for return value for insert fxn for sets
//	returnVal = PlaneSet.insert(plane);
//	if (returnVal.second == false) //returnVal.second is a bool that is false if didn't insert (dup) or true if did
//	{
//		std::cout << "Plane already inserted.  No duplicates." << std::endl;
//	}
//	else
//	{
//		std::cout << "Plane successfully inserted!" << std::endl;
//	}
//}

