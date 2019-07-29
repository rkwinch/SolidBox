#pragma once

#include <string>
#include <fstream>
#include <algorithm>
#include "Shape.h"
#include "ConnectionChannel.h"
#include "Menu.h"

class Surface {

protected:

	std::string m_stName = "";
	ConnectionChannel* m_channel;
	int m_nNumOfEdges = 0;

public:

	double m_dArea = 0;

	virtual double CalcArea() const = 0;
	virtual std::shared_ptr<Surface> GetCopy() = 0;
	virtual void Save(std::ofstream &outFile) = 0;

	double GetArea()
	{
		return m_dArea;
	}

	std::string GetName()
	{
		return m_stName;
	}

	ConnectionChannel* GetConnChannel()
	{
		return m_channel;
	}

	int GetNumOfEdges()
	{
		return m_nNumOfEdges;
	}
};
