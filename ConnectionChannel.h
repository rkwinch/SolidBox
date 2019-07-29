#pragma once

#include "Channel.h"
#include <set>
#include "SquarePlane.h"

class ConnectionChannel : public Channel {
private:
	std::set<SquarePlane*> PlaneSet;
	

public:
	ConnectionChannel();
	void Connect();
	void Disconnect();
	void Cleanup();
	~ConnectionChannel();
	void AddPlane(SquarePlane* plane); // sets PlaneSet with a set of SquarePlanes
	std::set<SquarePlane*> GetPlaneSet();
	


};



