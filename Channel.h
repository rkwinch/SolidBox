#pragma once
#include <set>

class SquarePlane;
class Channel {
	
public:

	virtual void Connect(std::set<std::shared_ptr<SquarePlane>> planeSet) = 0;
	virtual void Disconnect() = 0;
	virtual void Cleanup() = 0;

};


