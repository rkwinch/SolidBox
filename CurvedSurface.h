#pragma once

#include <string>
#include <memory>
#include "ConnectionChannel.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "Utility.h"
#include "ConnectionChannel.h"
#include "Plane.h"
//#define PI 3.14159

template<class T>
class CurvedSurface : public Plane<T> {

	//A curved surface is a plane-equivalent for curved surfaces (i.e. spheres).   

		friend class Utility;
		friend class SolidBox;

	public:
		static const int PI;

		CurvedSurface()
		{
			// empty
		}

		// parameterized constructor
		CurvedSurface(double radius, ConnectionChannel<T>* channel)
		{
			m_stName = Utility::CreateUniqueName("curvedSurf", nameIDCounter);
			m_dRadius = radius;
			m_dArea = CalcArea();
			this->m_channel = m_channel;
		}

		//defining the == operator for CurvedSurface to be based on comparisons of the name
		bool operator==(const CurvedSurface& plane) const
		{
			return (this->m_stName == plane.m_stName);
		}

		//defining the = operator for CurvedSurface to deep copy plane
		CurvedSurface& operator=(const CurvedSurface& plane)
		{
			// don't change name here
			m_dHeight = plane.m_dHeight;
			m_dLength = plane.m_dLength;
			m_nNumOfEdges = plane.m_nNumOfEdges;
			return *this;
		}

		//defining < operator for CurvedSurface to be based on comparisons of the name
		bool operator<(const CurvedSurface &plane) const
		{
			return (this->m_stName < plane.m_stName);
		}

		void SetName(std::string name)
		{
			m_stName = name;
		}

		std::string GetName()
		{
			return m_stName;
		}

		ConnectionChannel<T>* GetConnChannel()
		{
			return m_channel;
		}

		void SetNumOfEdges(int numOfEdges)
		{
			// empty.  Cannot change the # of edges
		}

		int GetNumOfEdges()
		{
			return m_nNumOfEdges;
		}

		void SetRadius(double radius)
		{
			m_dRadius = radius;
		}

		double GetRadius()
		{
			return m_dRadius;
		}

		void Save(std::ofstream &outFile)
		{
			outFile << name << ";";
			outFile << m_dRadius << ";" << numOfEdges << ";";
		}

	private:
		double m_dRadius;

		double CalcArea() override
		{
			return (4 * PI * pow(m_dRadius, 2);
		}
};

template<class T>
const int CurvedSurface<T>::PI = 3.14159;