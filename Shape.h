#pragma once

#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Utility.h"
#include "ConnectionChannel.h"
#include "Surface.h"

class Shape {

protected:

	ConnectionChannel m_channel;
	std::string m_stName = "";
	bool m_bHasConnection = false; // flag for checking if the shape has a connection
	double m_dVolume = 0;
	double m_dSurfaceArea = 0;

public:

	int m_nNumOfSurfaces = 0;

	virtual std::set<std::shared_ptr<Surface>> GetSurfacesCopy() = 0;
	virtual void Save(std::ofstream &outFile) = 0;
	virtual void Delete() = 0;
	virtual void CalcVol() = 0;
	virtual void CalcSA() = 0;
	virtual std::string PrintShapeInfo() = 0;
	virtual std::string PrintPlanesInfo() = 0;

	Shape() : m_channel(this) {}

	Shape(const Shape& shape) : m_channel(shape.m_channel) {}

	bool operator==(const Shape &shape) const
	{
		return (m_stName == shape.m_stName);
	}

	bool operator<(const Shape &shape) const
	{
		return (m_stName < shape.m_stName);
	}

	void SetName(std::string name)
	{
		m_stName = name;
	}

	std::string GetName()
	{
		return m_stName;
	}
	
	double GetSA()
	{
		return m_dSurfaceArea;
	}

	double GetVol()
	{
		return m_dVolume;
	}

	int GetSurfaceCount()
	{
		return m_nNumOfSurfaces;
	}

	bool GetHasConnection()
	{
		return m_bHasConnection;
	}

	std::string GetConnName()
	{
		return m_channel.GetName();
	}

	ConnectionChannel* GetConnChannel()
	{
		return &m_channel;
	}	

	std::string PrintChannelInfo()
	{
		bool bIsFirstSurface = true;
		std::string strHeader = "Channel:";
		std::string output = "";
		std::ostringstream stream;

		stream << Utility::PrintHeader(strHeader);
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Channel name:" << m_channel.GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Associated Shape name:" << m_channel.GetShape()->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Associated Surface name(s):";

		for (auto surfacePtr : m_channel.GetSurfaceSet())
		{
			if (bIsFirstSurface == true)
			{
				stream << std::left << surfacePtr->GetName() << std::endl;
				bIsFirstSurface = false;
			}
			else
			{
				stream << Utility::PrintChar(' ', Utility::PRINTING_WIDTH + 5);
				stream << std::left << surfacePtr->GetName() << std::endl;
			}
		}

		stream << std::endl;
		return output = stream.str();
	}

	std::string PrintShapeDebugInfo()
	{
		std::string output = "";
		std::ostringstream stream;

		stream << PrintShapeInfo();
		stream << PrintChannelInfo();
		stream << PrintPlanesInfo();
		return output = stream.str();
	}
};