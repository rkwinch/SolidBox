#pragma once

#include <set>
#include <afx.h>
#include <memory>
#include <vector>
#include "Channel.h"

class SquarePlane;

template<class T>
class ConnectionChannel : public CObject, public Channel {

	friend class Utility;

private:
	std::set<std::shared_ptr<SquarePlane>> planeSet;
	std::string name;
	T* cube;
	static int nameIDCounter;

	void SetName(std::string name)
	{
		this->name = name;
	}

public:
	DECLARE_SERIAL(ConnectionChannel)
	IMPLEMENT_SERIAL(ConnectionChannel<T>, CObject, 0)

	//default constructor
	ConnectionChannel()
	{
		// only here because of CObject
	}

	//parameterized constructor
	ConnectionChannel(T* cube)
	{
		name = Utility::CreateUniqueName("connectionChannel", nameIDCounter);
		this->cube = cube; // want the cube member here to have same address of cube
							// it is constructed from 
		// **will get planeSet as a SolidBox is being constructed**
	}

	//copy constructor
	ConnectionChannel(const ConnectionChannel& channel)
	{
		// new name for copy constructor was requested (will not copy current name)
		// (will probably not want to call copy constructor.  pass by reference instead)
		std::string name = Utility::CreateUniqueName("cube", nameIDCounter);

		// allocating new memory for the copy using the length (same as height for SquarePlane
		// implementation) of a SquarePlane* in the set of channel
		for (auto planePtr : planeSet)
		{
			std::shared_ptr<SquarePlane> copyPlanePtr = std::make_shared<SquarePlane>(planePtr->GetSqPlaneHeight(), this);
			this->planeSet.insert(copyPlanePtr);
		}

		//setting cube ptr
		cube = channel.cube;
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
		std::set<std::shared_ptr<SquarePlane>>::iterator channelPlaneSetItr = channel.planeSet.begin();
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
	void Connect() override
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
		//not really needed since using shared_ptr for SquarePlanes
	}

	std::set<std::shared_ptr<SquarePlane>> GetPlaneSet()
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


	void Serialize(CArchive& ar) {
		CObject::Serialize(ar);

		if (ar.IsStoring())
		{
			//ex: ar << empID << empName << age;
		}

		else
		{
			//ex: ar >> empID >> empName >> age;
		}

	}
};

