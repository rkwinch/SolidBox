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
#include "RectPlane.h"
#include "CurvedSurface.h"

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
	m_stName = Utility::CreateUniqueName("connectionChannel", m_nNameIDCounter);

	// allocating new memory for the copy 
	for (auto surface : channel.m_surfaceSet)
	{
		auto copy = surface->GetCopy();
		copy->SetConnChannel(this);
		m_surfaceSet.insert(copy);
	}
}

//operator =
ConnectionChannel& ConnectionChannel::operator=(ConnectionChannel &channel)
{
	//don't change name
	std::set<std::shared_ptr<Surface>>::iterator surfaceSetItr = channel.m_surfaceSet.begin();
	try {
		for (auto surface : m_surfaceSet)
		{
			if (surface->GetName().find("plane") != std::string::npos)
			{
				auto rectPlane = dynamic_cast<RectPlane*>(surface.get());
				auto channelRectPlane = dynamic_cast<RectPlane*>((*surfaceSetItr).get());
				*rectPlane = *channelRectPlane;
			}
			else if (surface->GetName().find("curvedSurf") != std::string::npos)
			{
				auto curvedSurface = dynamic_cast<CurvedSurface*>(surface.get());
				auto channelCurvedSurface = dynamic_cast<CurvedSurface*>((*surfaceSetItr).get());
				*curvedSurface = *channelCurvedSurface;
			}
			else
			{
				throw std::exception("Unknown surface detected");
			}

			++surfaceSetItr;
		}
	}
	catch (std::exception e)
	{
		Utility::Display(e.what());
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
		auto insert = m_surfaceSet.insert(surface);
		
		if (insert.second == false) // checking if already in set (has same name, need unique name, so throw exception)
		{
			throw std::exception("Cannot make Shape.  Duplicate surfaces detected.");
		}
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

void ConnectionChannel::SetShape(Shape* shape)
{
	m_shape = shape;
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