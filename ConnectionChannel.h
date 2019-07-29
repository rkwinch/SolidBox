#pragma once

#include <set>
#include <string>
#include <fstream>
#include <memory>

class Shape;
class Surface;

class ConnectionChannel {

private:

	std::string m_stName = "";
	Shape* m_shape;
	std::set<std::shared_ptr<Surface>> m_surfaceSet;

public:

	static int m_nNameIDCounter;

	ConnectionChannel(Shape* shape);
	~ConnectionChannel();
	ConnectionChannel(const ConnectionChannel& channel);
	ConnectionChannel& operator=(ConnectionChannel &channel);
	bool operator==(const ConnectionChannel& channel) const;
	bool operator<(const ConnectionChannel& channel) const;
	void Connect(std::set<std::shared_ptr<Surface>> surfaceSet);
	void Disconnect();
	void Cleanup();
	void SetName(std::string name);
	std::string GetName();
	std::set<std::shared_ptr<Surface>> GetSurfaceSet();
	Shape* GetShape();
	void Save(std::ofstream &outFile);
};