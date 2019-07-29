#pragma once

#include <set>
#include <afx.h>
#include <memory>
#include <vector>
#include "Channel.h"
#include "Shape.h"
#include <iostream>
#include <set>
#include "Utility.h"
#include <afx.h>
#include <memory>
#include <vector>

template<class T>
class ConnectionChannel : public CObject, public Channel<T> {

	friend class Utility;

private:
	std::set<std::shared_ptr<T>> planeSet;
	std::string name;
	T* shape;
	static int nameIDCounter = 1;

	void SetName(std::string name)
	{
		this->name = name;
	}

public:
	DECLARE_SERIAL(ConnectionChannel)
	IMPLEMENT_SERIAL(ConnectionChannel, CObject, 0)

	void Serialize(CArchive& ar)
	{
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

	ConnectionChannel()
	{
		// only here because of CObject
	}

	std::string GetConnName()
	{
		return name;
	}

	bool operator==(const ConnectionChannel& channel) const
	{
		return (name == channel.name);
	}

	bool operator<(const ConnectionChannel& channel) const
	{
		return (name < channel.name);
	}

	ConnectionChannel& operator=(ConnectionChannel &channel)
	{
		//don't change name
		std::set<std::shared_ptr<T>>::iterator channelPlaneSetItr = channel.planeSet.begin();
		for (auto plane : planeSet)
		{
			*plane = **(channelPlaneSetItr);
			++channelPlaneSetItr;
		}

		return *this;
	}

	//adding plane(s) to the connection
	void Connect(std::set<std::shared_ptr<T>> planeSet)
	{
		for (auto plane : planeSet)
		{
			this->planeSet.insert(plane);
		}
	}

	//removing plane(s) from the connection
	void Disconnect(std::set<std::shared_ptr<T>> planeSet)
	{
		for (auto plane : planeSet)
		{
			plane = nullptr;
		}
	}

	//cleans up memory when disconnecting a channel
	void Cleanup(T* plane)
	{
		//not really needed since using shared_ptr for SquarePlanes
	}

	// parameterized constructor
	ConnectionChannel(T* shape)
	{
		name = Utility::CreateUniqueName("connectionChannel", nameIDCounter);
		this->shape = shape; // want the shape member here to have same address of shape
						     // it is constructed from 
		// **will get planeSet as shape is being constructed**
	}

	virtual void Connect(std::set<std::shared_ptr<T>> planeSet) override;
	virtual void Disconnect(std::set<std::shared_ptr<T>> planeSet) override;
	virtual void Cleanup(T* plane) override;

	//destructor
	~ConnectionChannel()
	{
		// don't need to do anything here since memory is handled via smart pointers
	}
	
	std::set<std::shared_ptr<T>> GetPlaneSet();

	//copy constructor
	ConnectionChannel(const ConnectionChannel& channel)
	{
		// new name for copy constructor was requested (will not copy current name)
		// (will probably not want to call copy constructor.  pass by reference instead)
		name = Utility::CreateUniqueName("channel", nameIDCounter);

		// allocating new memory for the copy using the length 
		for (auto planePtr : planeSet)
		{
			std::shared_ptr<T> copyPlanePtr = std::make_shared<T>(planePtr->GetSqPlaneHeight(), this);
			this->planeSet.insert(copyPlanePtr);
		}

		//setting cube ptr
		shape = channel.shape;
	}

	T* GetShape()
	{
		return shape;
	}
};