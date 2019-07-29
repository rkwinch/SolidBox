#pragma once

#include <string>
#include <set>
#include <memory>
#include <vector>
#include "Shape.h"
#include "ConnectionChannel.h"
//#include "RectPlane.h"

template<class T>
class RectPlane;

class SolidBox : public Shape<SolidBox, RectPlane<SolidBox>> {

	friend class Utility;

public:
	SolidBox();
	~SolidBox();
	SolidBox(SolidBox&);
	SolidBox(double sideLength);
	double GetSideLength();
	SolidBox& operator=(SolidBox &cube);
	static void Load(std::vector<std::string>::iterator &itr, const int &vecSize);
	void Save(std::ofstream &outFile) override;
	std::set<std::shared_ptr<RectPlane<SolidBox>>> GetSurfacesCopy() override;
	int GetSurfaceCount() override;
	std::vector<std::shared_ptr<SolidBox>> GetShapeVec() override;
	static std::vector<std::shared_ptr<SolidBox>> m_shapeVec;

private:
	double m_dSideLength;
	static const int m_nNumOfSurfaces = 6;
};

