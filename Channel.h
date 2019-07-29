#pragma once
#include "stdafx.h"

class Channel {
private:
	//template<class T>
	//virtual std::vector<T*> groupOfPlanes = 0;
public:
	virtual void Connect()=0;
	virtual void Disconnect()=0;
	virtual void Cleanup()=0;

};
