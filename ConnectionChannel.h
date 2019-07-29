#pragma once

#include <set>
#include "Channel.h"

template<class T, class M>
class ConnectionChannel : public Channel<M> {

	friend class Utility;

private:
	std::set<std::shared_ptr<M>> m_surfaceSet;
	

public:
	//default constructor
	ConnectionChannel()
	{
		// only here because of CObject
	}

	//parameterized constructor
	ConnectionChannel(T* shape)
	{
		m_stName = Utility::CreateUniqueName("connectionChannel", m_nNameIDCounter);
		m_shape = shape; // want the shape member here to have same address of shape
						 // it is constructed from 
		// **will get surfaceSet as a shape is being constructed**
	}

	//copy constructor    (new = old)
	ConnectionChannel(const ConnectionChannel& channel)
	{
		// new name for copy constructor was requested (will not copy current name)
		// (will probably not want to call copy constructor.  pass by reference instead)
		m_stName = Utility::CreateUniqueName("channel", m_nNameIDCounter);

		// allocating new memory for the copy 
		for (auto surface : channel.m_surfaceSet)
		{
			std::shared_ptr<M> copy = surface->GetCopy();
			this->surfaceSet.insert(copy);
		}
		
		//setting shape ptr
		m_shape = channel.m_shape;
	}
	
	//destructor
	~ConnectionChannel()
	{
		// don't need to do anything here since memory is handled via smart pointers
	}

	//operator =
	ConnectionChannel& operator=(ConnectionChannel &channel)
	{
		//don't change name
		std::set<std::shared_ptr<M>>::iterator channelPlaneSetItr = channel.planeSet.begin();
		for (auto plane : planeSet)
		{
			*plane = **(channelPlaneSetItr);
			++channelPlaneSetItr;
		}

		return *this;
	}

	//operator ==
	bool operator==(const ConnectionChannel& channel) const
	{
		return (m_stName == channel.m_stName);
	}

	//operator <
	bool operator<(const ConnectionChannel& channel) const
	{
		return (m_stName < channel.m_stName);
	}

	//adding a plane to the connection
	void Connect(std::set<std::shared_ptr<M>> surfaceSet) override
	{
		for (auto surface : surfaceSet)
		{
			this->surfaceSet.insert(surface);
		}
	}

	//sets the planes to null, effectively "disconnecting them from the ConnectionChannel
	void Disconnect() override
	{
		for (auto surface : m_surfaceSet)
		{
			surface = nullptr;
		}
	}

	//cleans up memory when disconnecting a channel
	void Cleanup() override
	{
		//not really needed since using shared_ptr for surfaces
	}

	std::set<std::shared_ptr<M>> GetSurfaceSet()
	{
		return m_surfaceSet;
	}

	T* GetShape()
	{
		return m_shape;
	}

	std::string GetConnName()
	{
		return m_stName;
	}

	T* GetShapePtr()
	{
		return m_shape;
	}

	void SetName(std::string name)
	{
		m_stName = name;
	}

	void Save(std::ofstream &outFile) override
	{
		outFile << m_stName << ";";
		for (auto surface : m_surfaceSet) // can be RectPlane or CurvedSurface
		{
			surface->Save(outFile); // takes care of all of the surfaces
		}
	}
};

