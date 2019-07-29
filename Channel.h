#pragma once

class Channel {
	
public:

	virtual void Connect() = 0;
	virtual void Disconnect() = 0;
	virtual void Cleanup() = 0;

};


