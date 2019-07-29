#pragma once

#include <string>
#include <set>
#include <vector>
#include "Shape.h"

class RectPlane;
class ConnectionChannel;

class SolidBox : public Shape {

private:

	double m_dSideLength;

public:
	static const int m_nSurfaces;
	static std::vector<std::shared_ptr<SolidBox>> m_shapeVec;

	SolidBox(double sideLength);
	~SolidBox();
	SolidBox(SolidBox&);
	SolidBox& operator=(SolidBox &cube);
	void Delete() override;
	double GetSideLength();
	std::set<std::shared_ptr<Surface>> GetSurfacesCopy() override;
	std::vector<std::shared_ptr<SolidBox>> GetShapeVec();
	void Save(std::ofstream &outFile) override;
	static void Create();
	static void PrintSolids();
	static void Load(std::vector<std::string>::iterator &itr, const int &vecSize);
};
