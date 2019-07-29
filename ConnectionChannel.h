#pragma once

#include "Channel.h"
#include <set>
//#include "SquarePlane.h"
//#include "SolidBox.h"

class SquarePlane;
class SolidBox;

class ConnectionChannel /*: public Channel*/ {
private:
	std::set<SquarePlane*> planeSet;
	std::string name;
	SolidBox* cube;
	static int nameIDCounter;
	static std::set<std::string> channelNames;

public:
	std::string GetConnName();
	bool operator==(const ConnectionChannel& channel) const;
	bool operator<(const ConnectionChannel& channel) const;
	ConnectionChannel(SolidBox* cube);
	//template <class T>
	void Connect(SquarePlane* plane) /*override*/;
	//template <class T>
	void Disconnect(SquarePlane* plane) /*override*/;
	//template <class T>
	void Cleanup(SquarePlane* plane) /*override*/;
	~ConnectionChannel();
	std::set<SquarePlane*> GetPlaneSet();
	static std::set<std::string>* GetChannelNames();
	ConnectionChannel(const ConnectionChannel& channel);

};
