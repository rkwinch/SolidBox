#pragma once

#include <string>
#include <fstream>
#include <vector>

class ConnectionChannel;
class Shape;

class Surface {

protected:

	std::string m_stName = "";
	ConnectionChannel* m_channel;
	int m_nNumOfEdges = 0;

public:

	double m_dArea = 0;

	virtual void CalcArea() = 0;
	virtual std::shared_ptr<Surface> GetCopy() const = 0;
	virtual void Save(std::ofstream &outFile) const = 0;

	Surface() {}

	Surface(Surface& surface) {}

	double GetArea() const
	{
		return m_dArea;
	}

	std::string GetName() const
	{
		return m_stName;
	}

	void SetConnChannel(ConnectionChannel* channel)
	{
		m_channel = channel;
	}

	ConnectionChannel* GetConnChannel() const
	{
		return m_channel;
	}

	int GetNumOfEdges() const
	{
		return m_nNumOfEdges;
	}
};