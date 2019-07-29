#include <string>
#include <set>
#include <iostream>
#include <map>
#include <fstream>
#include <memory>
#include <algorithm>
#include <vector>
#include "Utility.h"
#include "Sphere.h"
#include "ConnectionChannel.h"

// default constructor 
Sphere::Sphere() : m_channel(this)
{
	// not needed
}

// destructor
Sphere::~Sphere()
{
	// don't need to do anything here since memory is handled via smart pointers
}

// parameterized constructor
Sphere::Sphere(double m_dRadius) : m_channel(this)
{
	m_stName = Utility::CreateUniqueName("sphere", m_nNameIDCounter);
	std::set<std::shared_ptr<RectPlane>> RectPlaneSet;

	//making 6 planes to go with the cube
	for (int i = 0; i < SolidBox::planesPerSolidBox; ++i)
	{
		std::cout << "about to make a plane with sidelength of: " << sideLength << std::endl;
		std::shared_ptr<RectPlane> plane = std::make_shared<RectPlane>(sideLength, &channel);
		RectPlaneSet.insert(plane);
	}

	channel.Connect(RectPlaneSet);

	//if all planes inserted correctly, then a proper ChannelConnection has been made
	this->m_dRadius = m_dRadius;
	m_bHasConnection = true;
}

// copy constructor
SolidBox::SolidBox(SolidBox& other)
{
	// don't change name
	m_dSideLength = other.m_dSideLength;
	ConnectionChannel<SolidBox> channel = other.channel;
	bHasConnection = other.bHasConnection; // flag for checking if the SolidBox has a connection

}

// since it was requested that old object be discarded afterwards, this is
// more like a move.  That is why this is not const. '=' is a move.
SolidBox& SolidBox::operator=(SolidBox &cube)
{
	m_dSideLength = cube.m_dSideLength;
	channel = cube.channel;
	bHasConnection = cube.bHasConnection;
	cube.Delete(); //**deleting items on right side of = operator**
	return *this;
}

double SolidBox::GetSideLength()
{
	return m_dSideLength;
}