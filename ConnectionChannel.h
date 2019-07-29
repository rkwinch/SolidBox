#pragma once

#include "Channel.h"
#include <set>
#include <afx.h>
#include <memory>

class SquarePlane;
class SolidBox;

class ConnectionChannel : public CObject/*, public Channel*/ {

	friend class Utility;
	friend class SolidBox;

private:
	std::set<std::shared_ptr<SquarePlane>> planeSet;
	std::string name;
	std::shared_ptr<SolidBox> cube;
	static int nameIDCounter;
	static std::set<std::string> channelNames;


public:
	DECLARE_SERIAL(ConnectionChannel)
	void Serialize(CArchive& ar);
	ConnectionChannel();

	std::string GetConnName();
	bool operator==(const ConnectionChannel& channel) const;
	bool operator<(const ConnectionChannel& channel) const;
	ConnectionChannel& operator=(ConnectionChannel &channel);
	ConnectionChannel(std::shared_ptr<SolidBox> cube);
	void Connect(std::set<std::shared_ptr<SquarePlane>> planeSet) /*override*/;
	void Disconnect(std::set<std::shared_ptr<SquarePlane>> planeSet) /*override*/;
	void Cleanup(SquarePlane* plane) /*override*/;
	~ConnectionChannel();
	std::set<std::shared_ptr<SquarePlane>> GetPlaneSet();
	static std::set<std::string>* GetChannelNames();
	ConnectionChannel(const ConnectionChannel& channel);
	std::shared_ptr<SolidBox> GetSolidBox();

};

