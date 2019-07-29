#pragma once

#include <string>
#include <set>
#include <vector>
#include <fstream>
#include "Shape.h"

class RectPrism : public Shape {

private:

	double m_dLength = 0;
	double m_dWidth = 0;
	double m_dHeight = 0;


public:
	static const int m_nSurfaces;
	static std::vector<std::shared_ptr<RectPrism>> m_shapeVec;
	static int m_nNameIDCounter;

	RectPrism(double length, double width, double height);
	~RectPrism();
	RectPrism(RectPrism& other);
	RectPrism& operator=(RectPrism &cube);
	void Delete() override;
	void CalcVol() override;
	void CalcSA() override;
	double GetLength();
	double GetHeight();
	double GetWidth();
	std::set<std::shared_ptr<Surface>> GetSurfacesCopy() override;
	std::vector<std::shared_ptr<RectPrism>> GetShapeVec();
	void Save(std::ofstream &outFile) override;
	static void Create();
	static void PrintSolids();
	static void Load(std::vector<std::string>::iterator &itr);
	static bool Move();
};