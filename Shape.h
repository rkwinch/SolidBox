#pragma once

#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "Utility.h"
#include "ConnectionChannel.h"
#include "Surface.h"

//abstract.  Don't make instances of Shape.

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

	Shape() : m_channel(this) 
	{
		// empty 
	}

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
};
