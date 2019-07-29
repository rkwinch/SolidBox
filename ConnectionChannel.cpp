#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "ConnectionChannel.h"
#include "Shape.h"
#include "Surface.h"
#include "Menu.h"
#include "Utility.h"

int ConnectionChannel::m_nNameIDCounter = 0;

//parameterized constructor
ConnectionChannel::ConnectionChannel(Shape* shape)
{
	m_stName = Utility::CreateUniqueName("connectionChannel", m_nNameIDCounter);
	m_shape = shape; // want the shape member here to have same address of shape
					 // it is constructed from 
	// **will get surfaceSet as a shape is being constructed**
}

//destructor
ConnectionChannel::~ConnectionChannel()
{
	// don't need to do anything here since memory is handled via smart pointers
}

//copy constructor    (new = old)
ConnectionChannel::ConnectionChannel(const ConnectionChannel& channel)
{
	// new name for copy constructor was requested (will not copy current name)
	// (will probably not want to call copy constructor.  pass by reference instead)
	m_stName = Utility::CreateUniqueName("channel", m_nNameIDCounter);

	// allocating new memory for the copy 
	for (auto surface : channel.m_surfaceSet)
	{
		std::shared_ptr<Surface> copy = surface->GetCopy();
		m_surfaceSet.insert(copy);
	}

	//setting shape ptr
	m_shape = channel.m_shape;
}

//operator =
ConnectionChannel& ConnectionChannel::operator=(ConnectionChannel &channel)
{
	//don't change name
	std::set<std::shared_ptr<Surface>>::iterator surfaceSetItr = m_surfaceSet.begin();

	for (auto surface : m_surfaceSet)
	{
		*surface = **(surfaceSetItr);
		++surfaceSetItr;
	}

	return *this;
}

//operator ==
bool ConnectionChannel::operator==(const ConnectionChannel& channel) const
{
	return (m_stName == channel.m_stName);
}

//operator <
bool ConnectionChannel::operator<(const ConnectionChannel& channel) const
{
	return (m_stName < channel.m_stName);
}

//adding a plane to the connection
void ConnectionChannel::Connect(std::set<std::shared_ptr<Surface>> surfaceSet)
{
	for (auto surface : surfaceSet)
	{
		m_surfaceSet.insert(surface);
	}
}

//sets the planes to null, effectively "disconnecting them from the ConnectionChannel
void ConnectionChannel::Disconnect()
{
	for (auto surface : m_surfaceSet)
	{
		surface = nullptr;
	}
}

//cleans up memory when disconnecting a channel
void ConnectionChannel::Cleanup()
{
	//not really needed since using shared_ptr for surfaces
}

void ConnectionChannel::SetName(std::string name)
{
	m_stName = name;
}

std::string ConnectionChannel::GetName()
{
	return this->m_stName;
}

std::set<std::shared_ptr<Surface>> ConnectionChannel::GetSurfaceSet()
{
	return this->m_surfaceSet;
}

Shape* ConnectionChannel::GetShape()
{
	return m_shape;
}
void ConnectionChannel::Save(std::ofstream &outFile)
{
	outFile << m_stName << ";";

	for (auto surface : m_surfaceSet) // can be RectPlane or CurvedSurface
	{
		surface->Save(outFile); // takes care of all of the surfaces
	}
}