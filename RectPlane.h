#pragma once

#include <string>
#include <memory>
#include "ConnectionChannel.h"
#include "Surface.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "Utility.h"
#include "ConnectionChannel.h"

//A rectangular (rect) plane is a plane that can be square or rectangular.  
//Each plane is a 2D rectangle that has 4 edges that are
//90' with a height and a length.

template<class T>
class RectPlane : public Plane<RectPlane<T>> {

	friend class Utility;
	friend class SolidBox;

public:
	static int RectPlane<T>::m_nNameIDCounter; // initialized at end of this header

	RectPlane()
	{
		// empty
	}

	// parameterized constructor
	RectPlane(double length, double height, ConnectionChannel<T, RectPlane>* channel)
	{
			m_stName = Utility::CreateUniqueName("plane", nameIDCounter);
			m_dLength = length;
			m_dHeight = height;
			m_nNumOfEdges = 4;
			m_dArea = m_dLength * m_dHeight;
			this->m_channel = m_channel;
	}

	//defining the == operator for RectPlane to be based on comparisons of the name of the plane
	bool operator==(const RectPlane& plane) const
	{
		return (this->m_stName == plane.m_stName);
	}

	//defining the = operator for RectPlane to deep copy plane
	RectPlane& operator=(const RectPlane& plane)
	{
			// don't change name here
			m_dHeight = plane.m_dHeight;
			m_dLength = plane.m_dLength;
			m_nNumOfEdges = plane.m_nNumOfEdges;
			m_dArea = plane.m_dArea;
			return *this;
	}

	//defining < operator for RectPlane to be based on comparisons of the name of the plane
	bool operator<(const RectPlane &plane) const
	{
		return (this->m_stName < plane.m_stName);
	}

	std::string GetSqPlaneName()
	{
		return m_stName;
	}

	double GetSqPlaneLength()
	{
		return m_dLength;
	}

	double GetSqPlaneHeight()
	{
		return m_dHeight;
	}

	ConnectionChannel<T, RectPlane>* GetConnChannel()
	{
		return m_channel;
	}

	int GetNumOfEdges()
	{
		return m_nNumOfEdges;
	}

	double GetArea()
	{
		return m_dArea;
	}

	void SetName(std::string name)
	{
		m_stName = name;
	}

	void SetLength(double length)
	{
		m_dLength = length;
	}

	void SetHeight(double height)
	{
		m_dHeight = height;
	}

	void SetNumOfEdges(int numOfEdges)
	{
		m_nNumOfEdges = numOfEdges;
	}

	double CalcArea() override
	{
		return (m_dHeight * m_dLength);
	}

	void Save(std::ofstream &outFile)
	{
		outFile << m_stName << ";";
		outFile << m_dHeight << ";" << m_dLength << ";" << m_nNumOfEdges << ";";
	}
	
	std::shared_ptr<RectPlane> GetCopy() override
	{
		return std::make_shared<RectPlane<SolidBox>>(m_dRadius, m_channel);
	}

private:
	double m_dHeight;
	double m_dLength;
};

template<class T>
int RectPlane<T>::m_nNameIDCounter = 0;