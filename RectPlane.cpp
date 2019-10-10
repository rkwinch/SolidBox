#include <string>
#include <iostream>
#include <fstream>
#include "RectPlane.h"
#include "Utility.h"
#include "ConnectionChannel.h"

int RectPlane::m_nNameIDCounter = 0;

// parameterized constructor
RectPlane::RectPlane(double length, double height, ConnectionChannel* channel)
{
	m_stName = Utility::CreateUniqueName("plane", m_nNameIDCounter);
	m_dLength = length;
	m_dHeight = height;
	m_nNumOfEdges = 4;
	CalcArea();
	m_channel = channel;
}

// copy constructor
RectPlane::RectPlane(const RectPlane &rectPlane)
{
	// don't change name
	if (m_stName.length() == 0)
	{
		m_stName = Utility::CreateUniqueName("plane", m_nNameIDCounter);
	}

	m_nNumOfEdges = rectPlane.m_nNumOfEdges;
	m_dLength = rectPlane.m_dLength;
	m_dHeight = rectPlane.m_dHeight;
	m_channel = rectPlane.m_channel;
	CalcArea();
}

//defining the = operator for RectPlane to deep copy plane
RectPlane& RectPlane::operator=(RectPlane& plane)
{
	// don't change name here
	m_dHeight = plane.m_dHeight;
	m_dLength = plane.m_dLength;
	m_nNumOfEdges = plane.m_nNumOfEdges;
	m_dArea = plane.m_dArea;	
	return *this;
}

//defining the == operator for RectPlane to be based on comparisons of the name of the plane
bool RectPlane::operator==(const RectPlane& plane) const
{
	return (m_stName == plane.m_stName);
}

//defining < operator for RectPlane to be based on comparisons of the name of the plane
bool RectPlane::operator<(const RectPlane &plane) const
{
	return (m_stName < plane.m_stName);
}

void RectPlane::CalcArea()
{
	m_dArea = m_dHeight * m_dLength;
}

void RectPlane::SetName(std::string name)
{
	m_stName = name;
}

double RectPlane::GetLength() const
{
	return m_dLength;
}

double RectPlane::GetHeight() const
{
	return m_dHeight;
}

std::shared_ptr<Surface> RectPlane::GetCopy() const
{
	return std::make_shared<RectPlane>(*this);
}

void RectPlane::Save(std::ofstream &outFile) const
{
	outFile << m_stName << ";";
	outFile << m_dHeight << ";" << m_dLength << ";" << m_nNumOfEdges << ";";
}