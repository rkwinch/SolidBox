#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <fstream>
#include "Utility.h"
#include "Surface.h"

template<class T, class M>
class ConnectionChannel;

//A rectangular (rect) plane is a plane that can be square or rectangular.  
//Each plane is a 2D rectangle that has 4 edges that are
//90' with a height and a length.

template<class T>
class RectPlane : public Surface<T, RectPlane<T>> {

	friend class Utility;
	friend class SolidBox;

private:

	double m_dHeight;
	double m_dLength;

public:

	static int m_nNameIDCounter; // initialized at end of this header

	RectPlane()
	{
		// empty
	}

	// parameterized constructor
	RectPlane(double length, double height, ConnectionChannel<T, RectPlane>* channel)
	{
			this->m_stName = Surface<T, RectPlane<T>>::CreateUniqueName("plane", this->m_nNameIDCounter);
			this->m_dLength = length;
			this->m_dHeight = height;
			this->m_nNumOfEdges = 4;
			this->m_dArea = this->m_dLength * this->m_dHeight;
			this->m_channel = channel;
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
			this->m_dHeight = plane.m_dHeight;
			this->m_dLength = plane.m_dLength;
			this->m_nNumOfEdges = plane.m_nNumOfEdges;
			this->m_dArea = plane.m_dArea;
			*(this->m_channel) = *(plane.m_channel);
			return *this;
	}

	//defining < operator for RectPlane to be based on comparisons of the name of the plane
	bool operator<(const RectPlane &plane) const
	{
		return (this->m_stName < plane.m_stName);
	}

	double GetSqPlaneLength()
	{
		return this->m_dLength;
	}

	double GetSqPlaneHeight()
	{
		return this->m_dHeight;
	}

	ConnectionChannel<T, RectPlane>* GetConnChannel()
	{
		return this->m_channel;
	}

	int GetNumOfEdges()
	{
		return this->m_nNumOfEdges;
	}

	double GetArea()
	{
		return this->m_dArea;
	}

	void SetName(std::string name)
	{
		this->m_stName = name;
	}

	void SetLength(double length)
	{
		this->m_dLength = length;
	}

	void SetHeight(double height)
	{
		this->m_dHeight = height;
	}

	void SetNumOfEdges(int numOfEdges)
	{
		this->m_nNumOfEdges = numOfEdges;
	}

	double CalcArea() const override
	{
		return (this->m_dHeight * this->m_dLength);
	}

	void Save(std::ofstream &outFile) override
	{
		outFile << this->m_stName << ";";
		outFile << this->m_dHeight << ";" << this->m_dLength << ";" << this->m_nNumOfEdges << ";";
	}
	
	std::shared_ptr<RectPlane> GetCopy() override
	{
		return std::make_shared<RectPlane<T>>(this->m_dLength, this->m_dHeight, this->m_channel);
	}

};

template<class T>
int RectPlane<T>::m_nNameIDCounter = 0;