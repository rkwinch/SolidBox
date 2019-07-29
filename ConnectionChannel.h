#pragma once
#include "stdafx.h"
#include "Channel.h"
#include <set>
#include "SolidBox.h"

class ConnectionChannel : public Channel {
private:
	std::set<SolidBox> CubeSet;
	std::set<SquarePlane> PlaneSet;
	std::set<std::set<SquarePlane>> PlanesOfCubes;

public:
	ConnectionChannel();
	void Connect();
	void Disconnect();
	void Cleanup();
	~ConnectionChannel();
	void AddPlanes(SquarePlane PlaneSet);
	void AddPlaneToSet(SquarePlane plane);

};

//public:
//	virtual void Connect();
//	virtual void Disconnect();
//	virtual void Cleanup();
