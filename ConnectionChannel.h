#pragma once

#include <set>
#include <memory>
#include <vector>
#include "Channel.h"

template<class T, class M>
class ConnectionChannel : public Channel<M> {

	friend class Utility;

private:
	std::set<std::shared_ptr<RectPlane>> planeSet;
	std::string name;
	T* m_shape;
	static int nameIDCounter;

public:
	//default constructor
	ConnectionChannel()
	{
		// only here because of CObject
	}

	//parameterized constructor
	ConnectionChannel(T* shape)
	{
		name = Utility::CreateUniqueName("connectionChannel", nameIDCounter);
		this->cube = cube; // want the cube member here to have same address of cube
							// it is constructed from 
		// **will get planeSet as a SolidBox is being constructed**
	}

	//copy constructor    (new = old)
	ConnectionChannel(const ConnectionChannel& channel)
	{
		// new name for copy constructor was requested (will not copy current name)
		// (will probably not want to call copy constructor.  pass by reference instead)
		std::string name = Utility::CreateUniqueName("channel", nameIDCounter);

		// allocating new memory for the copy 
		for (auto planePtr : channel.planeSet)
		{
			std::shared_ptr<M> copyPlanePtr = /*std::make_shared<M>(planePtr->GetSqPlaneHeight(), this)*/;
			this->planeSet.insert(copyPlanePtr);
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
		return (name == channel.name);
	}

	//operator <
	bool operator<(const ConnectionChannel& channel) const
	{
		return (name < channel.name);
	}

	//adding a plane to the connection
	void Connect(std::set<std::shared_ptr<M>> planeSet) override
	{
		for (auto plane : planeSet)
		{
			this->planeSet.insert(plane);
		}
	}

	//sets the planes to null, effectively "disconnecting them from the ConnectionChannel
	void Disconnect() override
	{
		for (auto plane : planeSet)
		{
			plane = nullptr;
		}
	}

	//cleans up memory when disconnecting a channel
	void Cleanup() override
	{
		//not really needed since using shared_ptr for planes
	}

	std::set<std::shared_ptr<RectPlane>> GetPlaneSet()
	{
		return planeSet;
	}

	std::string GetConnName()
	{
		return name;
	}

	T* GetShapePtr()
	{
		return cube;
	}

	void SetName(std::string name)
	{
		this->name = name;
	}

	void Save(std::ofstream &outFile)
	{
		outFile << name << ";";
		for (auto element : planeSet) // can be RectPlane or CurvedSurface
		{
			element->Save(outFile); // takes care of all of the square planes
		}
	}
};

