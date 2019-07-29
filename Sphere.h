#pragma once

#include <string>
#include <set>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Shape.h"
#include "CurvedSurface.h"
#include "ConnectionChannel.h"
#include "Utility.h"

class Sphere : public Shape<Sphere, CurvedSurface<Sphere>> {

	friend class Utility;

private:

	ConnectionChannel<Sphere, CurvedSurface<Sphere>> m_channel;
	double m_dRadius;
	static const int m_nSurfaces;

public:

	static const int m_nNumOfSurfaces = 1;
	static std::set<std::shared_ptr<Sphere>> m_surfaceSet;
	static std::vector<std::shared_ptr<Sphere>> m_shapeVec;

	ConnectionChannel<Sphere, CurvedSurface<Sphere>>* GetConnChannel()
	{
		return &m_channel;
	}

	std::string GetConnName()
	{
		return m_channel.GetName();
	}

	static void Create()
	{
		std::string strInput = "";
		double dRadius = 0.0;
		std::regex acceptableInputExpr("^\\s*([0-9]*\\s?[0-9]*\\s*$"); // looking for a number (if present)
																	   // with 0-1 decimals followed by a number (if present) while allowing spaces
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		std::cout << "What would you like the radius to be? (in mm)" << std::endl;
		std::cout << "ex: 0.125" << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		dRadius = std::stod(strInput); // converting string input into a double

		while (dRadius == 0.0)
		{
			std::cout << "Please input a value that is not 0" << std::endl;
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);
			dRadius = std::stod(strInput);
		}
		std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(dRadius);
		Sphere::m_shapeVec.push_back(sphere);
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
	}

	void Delete()
	{
		std::vector<std::shared_ptr<Sphere>>::iterator shapeVecItr = m_shapeVec.begin();
		// [&] is take by reference, arg type is shared ptr of surface type (solidbox or sphere at this point), return type is bool, 
		// predicate is check if the shapes are equivalent (same name by == operator)
		shapeVecItr = std::find_if(m_shapeVec.begin(), m_shapeVec.end(), [&](std::shared_ptr<Sphere> shape)->bool {return *shape == *this; });

		if (shapeVecItr == m_shapeVec.end())
		{
			std::cout << "Cannot delete solid.  Solid not found" << std::endl;
			return;
		}

		m_channel.Disconnect(); // setting surfaces in surfaceSet to null
		m_shapeVec.erase(shapeVecItr); // removing item from vector
	}

	int GetSurfaceCount() override
	{
		return Sphere::m_nNumOfSurfaces;
	}

	static void PrintSolids()
	{
		int count = 1;

		for (auto sphere : Sphere::m_shapeVec)
		{
			std::cout << count << ") " << sphere->m_stName << std::endl;
			++count;
		}
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
		m_stName = CreateUniqueName("sphere", m_nNameIDCounter);
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


};
