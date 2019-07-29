#pragma once

#include <set>

class SquarePlane;

template<class T>
class Channel {
	
public:
	virtual void Connect(std::set<std::shared_ptr<T>>) =0;
	virtual void Disconnect(std::set<std::shared_ptr<T>>) =0;
	virtual void Cleanup(T*) =0;

};
