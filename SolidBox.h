#pragma once

#include <string>
#include <set>
#include <vector>
#include <memory>
#include <fstream>
#include "Shape.h"

class RectPlane;
class ConnectionChannel;

class SolidBox : public Shape {

private:

	double m_dSideLength = 0;
	

public:

	static const int m_nSurfaces;
	static std::vector<std::shared_ptr<SolidBox>> m_shapeVec;
	static int m_nNameIDCounter;

	SolidBox(double sideLength);
	~SolidBox();
	SolidBox(const SolidBox& other);
	SolidBox& operator=(SolidBox &cube);
	void Delete() override;
	void CalcVol() override;
	void CalcSA() override;
	double GetSideLength();
	std::set<std::shared_ptr<Surface>> GetSurfacesCopy() override;
	std::vector<std::shared_ptr<SolidBox>> GetShapeVec();
	void Save(std::ofstream &outFile) override;
	static void Create();
	static std::string PrintSolids();
	static std::string PrintSolids(int &counter);
	static void Load(std::vector<std::string>::iterator &itr);
	static bool Move();
	static bool Move(std::shared_ptr<SolidBox> cube);
	std::string PrintShapeInfo() override;
	std::string PrintPlanesInfo() override;
};