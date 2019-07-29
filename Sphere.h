#pragma once

#include <string>
#include <set>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Shape.h"
#include "ConnectionChannel.h"
#include "CurvedSurface.h"
#include "Utility.h"



class Sphere : public Shape<Sphere, CurvedSurface<Sphere>> {

	friend class Utility;

public:
	static const int m_nNumOfSurfaces = 1;
	static std::set<std::shared_ptr<Sphere>> m_surfaceSet;
	static std::vector<std::shared_ptr<Sphere>> m_shapeVec;

	int GetSurfaceCount() override
	{
		return Sphere::m_nNumOfSurfaces;
	}

	// default constructor 
	Sphere() : m_channel(this) 
	{
		// not needed
	}

	// destructor
	~Sphere()
	{
		// don't need to do anything here since memory is handled via smart pointers
	}

	// copy constructor
	Sphere(Sphere& other)
	{
		// don't change name
		m_dRadius = other.m_dRadius;
		m_channel = other.m_channel;
		m_bHasConnection = other.m_bHasConnection; // flag for checking if the SolidBox has a connection
	}

	// parameterized constructor
	Sphere(double radius) : m_channel(this)
	{
		m_stName = Utility::CreateUniqueName("sphere", m_nNameIDCounter);
		std::set<std::shared_ptr<CurvedSurface<Sphere>>> surfaceSet;

		//making 1 surface to go with the sphere
		for (int i = 0; i < Sphere::m_nSurfaces; ++i)
		{
			std::shared_ptr<CurvedSurface<Sphere>> surface = std::make_shared<CurvedSurface<Sphere>>(radius, &m_channel);
			surfaceSet.insert(surface);
		}

		m_channel.Connect(surfaceSet);

		//if all planes inserted correctly, then a proper ConnectionChannel has been made
		this->m_dRadius = m_dRadius;
		m_bHasConnection = true;
	}

	double GetRadius()
	{
		return m_dRadius;
	}

	// since it was requested that old object be discarded afterwards, this is
	// more like a move.  That is why this is not const. '=' is a move.
	Sphere& operator=(Sphere &sphere)
	{
		m_dRadius = sphere.m_dRadius;
		m_channel = sphere.m_channel;
		m_bHasConnection = sphere.m_bHasConnection;
		sphere.Delete(); //**deleting items on right side of = operator**
		return *this;
	}

	std::vector<std::shared_ptr<Sphere>> GetShapeVec()
	{
		return Sphere::m_shapeVec;
	}

	void Save(std::ofstream &outFile) override
	{
		outFile << m_stName << ";";
		outFile << m_dRadius << ";" << m_bHasConnection << ";";
		m_channel.Save(outFile); // takes care of channel member and its associated planes
		outFile << "\n";
	}

	static void Load(std::vector<std::string>::iterator &itr, const int &vecSize)
	{
		std::string stName = "";
		double dRadius = 0;
		bool bHasConnection = false;
		int nNumOfEdges = 0;

		//getting members for sphere
		for (int ii = 0; ii < vecSize; ++ii)
		{
			stName = ""; // resetting name for spheres if there is more than one sphere
			stName = (*itr); std::cout << "in loadsphere should be a name: " << (*itr) << std::endl;
			itr++; std::cout << "in loadsphere should be a double: " << (*itr) << std::endl;
			dRadius = stod(*itr);
			itr++;
			bHasConnection = static_cast<bool>(stoi(*itr));
			itr++;

			//constructing a sphere with given radius and setting other params
			std::cout << "making a sphere from load with radius,hasconn,name: " << dRadius << " " << bHasConnection << " " << stName << std::endl;
			std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(dRadius);
			sphere->SetName(stName);
			stName = ""; // resetting name

			//getting connectionchannel name
			stName = (*itr);
			itr++;
			sphere->GetConnChannel()->SetName(stName);
			stName = ""; // resetting name

			//getting and setting members for square planes
			for (auto surface : sphere->GetConnChannel()->GetSurfaceSet())
			{
				stName = (*itr);
				itr++;
				dRadius = stod(*itr);
				itr++;
				nNumOfEdges = stoi(*itr);
				itr++; std::cout << "in loadsphere plane. last should be a ':' " << (*itr) << std::endl;
				surface->SetName(stName);
				stName = ""; // resetting name
				surface->SetRadius(dRadius);
				surface->SetNumOfEdges(nNumOfEdges);
			}
			itr++; // skipping ":" delimiter
			Sphere::m_shapeVec.push_back(sphere); // solid box object is completed now.
		}
	}
		
private:
	double m_dRadius;
	static const int m_nSurfaces;
};