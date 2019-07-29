#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "Surface.h"
#include "Utility.h"


template<class T, class M>
class ConnectionChannel;

template<class T>
class CurvedSurface : public Surface<T, CurvedSurface<T>> {

		//A curved surface is a plane-equivalent for curved surfaces (i.e. spheres).   

		friend class Utility;
		friend class SolidBox;

	public:
		static const int PI;
		static int m_nNameIDCounter;

		CurvedSurface()
		{
			// empty
		}

		// parameterized constructor
		CurvedSurface(double radius, ConnectionChannel<T, CurvedSurface>* channel)
		{
			this->m_stName = Utility::CreateUniqueName("curvedSurf", m_nNameIDCounter);
			this->m_dRadius = radius;
			this->m_dArea = CalcArea();
			this->m_channel = channel;
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
			this->m_dRadius = plane.m_dRadius;
			this->m_dArea = plane.CalcArea();
			this->m_nNumOfEdges = plane.m_nNumOfEdges;
			*(this->m_channel) = *(plane.m_channel);
			return *this;
		}

		//defining < operator for CurvedSurface to be based on comparisons of the name
		bool operator<(const CurvedSurface &plane) const
		{
			return (this->m_stName < plane.m_stName);
		}

		void SetName(std::string name)
		{
			this->m_stName = name;
		}

		std::string GetName()
		{
			return this->m_stName;
		}

		ConnectionChannel<T, CurvedSurface>* GetConnChannel()
		{
			return this->m_channel;
		}

		void SetNumOfEdges(int numOfEdges)
		{
			// empty.  Cannot change the # of edges
		}

		int GetNumOfEdges()
		{
			return this->m_nNumOfEdges;
		}

		void SetRadius(double radius)
		{
			this->m_dRadius = radius;
		}

		double GetRadius()
		{
			return this->m_dRadius;
		}

		void Save(std::ofstream &outFile)
		{
			outFile << this->m_stName << ";";
			outFile << this->m_dRadius << ";" << this->m_nNumOfEdges << ";";
		}

		std::shared_ptr<CurvedSurface> GetCopy() override
		{
			return std::make_shared<CurvedSurface>(this->m_dRadius, this->m_channel);
		}

	private:
		double m_dRadius;

		double CalcArea() override
		{
			return (4 * PI * pow(m_dRadius, 2));
		}
};

template<class T>
const int CurvedSurface<T>::PI = 3.14159;
template<class T>
int CurvedSurface<T>::m_nNameIDCounter = 0;