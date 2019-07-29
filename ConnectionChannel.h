#pragma once

#include <set>
#include "Channel.h"
#include "Shape.h"

template<class T, class M>
class ConnectionChannel : public Channel<T, M> {

	friend class Utility;

private:
	std::set<std::shared_ptr<Surface<T,M>>> m_surfaceSet;
	

public:
	//default constructor
	ConnectionChannel() 
	{
		// only here because of CObject
	}

	//parameterized constructor
	ConnectionChannel(T* shape) 
	{
		this->m_stName = Channel<T,M>::CreateUniqueName("connectionChannel", this->m_nNameIDCounter);
		this->m_shape = shape; // want the shape member here to have same address of shape
						 // it is constructed from 
		// **will get surfaceSet as a shape is being constructed**
	}

	//copy constructor    (new = old)
	ConnectionChannel(const ConnectionChannel& channel)
	{
		// new name for copy constructor was requested (will not copy current name)
		// (will probably not want to call copy constructor.  pass by reference instead)
		this->m_stName = Channel<T,M>::CreateUniqueName("channel", this->m_nNameIDCounter);

		// allocating new memory for the copy 
		for (auto surface : channel.m_surfaceSet)
		{
			std::shared_ptr<M> copy = surface->GetCopy();
			this->surfaceSet.insert(copy);
		}
		
		//setting shape ptr
		this->m_shape = channel.m_shape;
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
		typename std::set<std::shared_ptr<M>>::iterator channelPlaneSetItr = channel.m_surfaceSet.begin();
		for (auto surface: m_surfaceSet)
		{
			*surface = **(channelPlaneSetItr);
			++channelPlaneSetItr;
		}

		return *this;
	}

	//operator ==
	bool operator==(const ConnectionChannel& channel) const
	{
		return (this->m_stName == channel.m_stName);
	}

	//operator <
	bool operator<(const ConnectionChannel& channel) const
	{
		return (this->m_stName < channel.m_stName);
	}

	//adding a plane to the connection
	void Connect(std::set<std::shared_ptr<Surface<T,M>>> surfaceSet) override
	{
		for (auto surface : surfaceSet)
		{
			this->m_surfaceSet.insert(surface);
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

	std::set<std::shared_ptr<Surface<T,M>>> GetSurfaceSet()
	{
		return this->m_surfaceSet;
	}

	T* GetShape()
	{
		return this->m_shape;
	}

	std::string GetName()
	{
		return this->m_stName;
	}

	T* GetShapePtr()
	{
		return this->m_shape;
	}

	void SetName(std::string name)
	{
		this->m_stName = name;
	}

	void Save(std::ofstream &outFile) override
	{
		outFile << this->m_stName << ";";
		for (auto surface : m_surfaceSet) // can be RectPlane or CurvedSurface
		{
			surface->Save(outFile); // takes care of all of the surfaces
		}
	}
};

