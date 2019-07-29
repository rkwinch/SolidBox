#pragma once

#include "Channel.h"
#include <set>
#include "SolidBox.h"

class ConnectionChannel : public Channel {
private:
	std::set<SolidBox> CubeSet;
	std::set<SquarePlane> PlaneSet;
	//map of cubes to planes. cubes unique. plane sets don't have to be unique as of now?

public:
	ConnectionChannel();
	void Connect();
	void Disconnect();
	void Cleanup();
	~ConnectionChannel();
	void AddPlanes(const std::set<SquarePlane> PlaneSet);
	//void AddPlaneToSet(const SquarePlane& plane);

};



