#pragma once

#include <string>
#include <set>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Shape.h"
#include "Surface.h"
#include "CurvedSurface.h"
#include "ConnectionChannel.h"
#include "Utility.h"

class Sphere : public Shape {

private:

	double m_dRadius;
	
public:

	static const int m_nSurfaces;
	static std::vector<std::shared_ptr<Sphere>> m_shapeVec;
	static int m_nNameIDCounter;

	Sphere(double radius);
	~Sphere();
	Sphere(Sphere& other);
	Sphere& operator=(Sphere &sphere);
	void Delete() override;
	void CalcVol() override;
	void CalcSA() override;
	double GetRadius();
	std::set<std::shared_ptr<Surface>> GetSurfacesCopy() override;	
	std::vector<std::shared_ptr<Sphere>> GetShapeVec();
	void Save(std::ofstream &outFile) override;
	static void Create();
	static void PrintSolids();
	static void Load(std::vector<std::string>::iterator &itr);
};