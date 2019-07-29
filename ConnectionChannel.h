#pragma once

#include "Channel.h"
#include <set>
#include "SquarePlane.h"

class ConnectionChannel /*: public Channel*/ {
private:
	std::set<SquarePlane*> planeSet;
	std::string name;
	

public:
	std::string GetConnName();
	bool operator==(const ConnectionChannel& channel) const;
	bool operator<(const ConnectionChannel& channel) const;
	ConnectionChannel();
	//template <class T>
	void Connect(SquarePlane* plane) /*override*/;
	//template <class T>
	void Disconnect(SquarePlane* plane) /*override*/;
	//template <class T>
	void Cleanup(SquarePlane* plane) /*override*/;
	~ConnectionChannel();
	std::set<SquarePlane*> GetPlaneSet();
	


};



