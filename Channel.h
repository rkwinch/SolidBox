#pragma once
#include <set>

template<class T>
class RectPlane;

template<class T>
class Channel {
	
public:

	virtual void Connect(std::set<std::shared_ptr<T>> planeSet) = 0;
	virtual void Disconnect() = 0;
	virtual void Cleanup() = 0;

};


