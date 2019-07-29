#pragma once
#include "stdafx.h"
#include "ConnectionChannel.h"
#include "Shape.h"
#include "SquarePlane.h"
#include <iostream>


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
//void AddPlanes(const SquarePlane& PlaneSet);
void ConnectionChannel::AddPlanes(SquarePlane PlaneSet)
{

}
void ConnectionChannel::AddPlaneToSet(SquarePlane plane)
{
	std::pair<std::set<SquarePlane>::iterator, bool> returnVal; // for return value for insert fxn for maps
	returnVal = PlaneSet.insert(plane);
	if(returnVal.second == false) //returnVal.second is a bool that is false if didn't insert (dup) or true if did
	{
		std::cout << "Plane already inserted.  No duplicates." << std::endl;
	}
	else
	{
		std::cout << "Plane successfully inserted!" << std::endl;
	}
}

// deep copy of the pointers of all the planes
//for (auto planePtr : groupOfPlanes)
//{
//	this->groupOfPlanes.push_back(planePtr);
//}