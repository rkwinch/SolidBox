#pragma once

class Channel {
	// make a templated class later 
public:

	virtual void Connect() = 0;
	virtual void Disconnect() = 0;
	virtual void Cleanup() = 0;

};
