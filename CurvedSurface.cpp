#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "CurvedSurface.h"
#include "Sphere.h"
#include "Utility.h"

class ConnectionChannel;

const double CurvedSurface::PI = 3.14159;
int CurvedSurface::m_nNameIDCounter = 0;

// parameterized constructor
CurvedSurface::CurvedSurface(double radius, ConnectionChannel* channel)
{
	m_stName = Utility::CreateUniqueName("curvedSurf", m_nNameIDCounter);
	m_dRadius = radius;
	m_channel = channel;
	CalcArea();
}

// copy constructor
CurvedSurface::CurvedSurface(const CurvedSurface &curvedSurf)
{
	// don't change name
	if (m_stName.length() == 0)
	{
		m_stName = Utility::CreateUniqueName("curvedSurf", m_nNameIDCounter);
	}

	m_nNumOfEdges = curvedSurf.m_nNumOfEdges;
	m_dRadius = curvedSurf.m_dRadius;
	m_channel = curvedSurf.m_channel;
	CalcArea();
}

//defining the = operator for CurvedSurface to deep copy plane
CurvedSurface& CurvedSurface::operator=(CurvedSurface& plane)
{
	// don't change name here
	m_dRadius = plane.m_dRadius;
	m_nNumOfEdges = plane.m_nNumOfEdges; 
	m_dArea = plane.GetArea(); 
	return *this;
}

//defining the == operator for CurvedSurface to be based on comparisons of the name
bool CurvedSurface::operator==(const CurvedSurface& plane) const
{
	return (m_stName == plane.m_stName);
}

//defining < operator for CurvedSurface to be based on comparisons of the name
bool CurvedSurface::operator<(const CurvedSurface &plane) const
{
	return (m_stName < plane.m_stName);
}

void CurvedSurface::CalcArea() 
{
	m_dArea = 4.0 * PI * pow(m_dRadius, 2.0);
}

void CurvedSurface::SetName(std::string name)
{
	m_stName = name;
}

double CurvedSurface::GetRadius() const
{
	return m_dRadius;
}

ConnectionChannel* CurvedSurface::GetConnChannel() const
{
	return m_channel;
}

std::shared_ptr<Surface> CurvedSurface::GetCopy() const
{
	return std::make_shared<CurvedSurface>(*this);
}

void CurvedSurface::Save(std::ofstream &outFile) const
{
	outFile << m_stName << ";";
	outFile << m_dRadius << ";" << m_nNumOfEdges << ";";
}