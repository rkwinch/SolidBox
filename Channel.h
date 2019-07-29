#pragma once

class Channel {

public:
	//template <class T>
	virtual void Connect()=0;
	//template <class T>
	virtual void Disconnect() = 0;
	//template <class T>
	virtual void Cleanup() = 0;

};

