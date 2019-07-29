#pragma once

#include <string>
#include <set>
#include <vector>
#include "Shape.h"
#include "ConnectionChannel.h"

template<class T>
class RectPlane;

class SolidBox : public Shape<SolidBox, RectPlane<SolidBox>> {

	friend class Utility;

public:

	SolidBox();
	~SolidBox();
	SolidBox(SolidBox&);
	SolidBox(double sideLength);
	SolidBox& operator=(SolidBox &cube);
	double GetSideLength();
	int GetSurfaceCount() override;
	std::set<std::shared_ptr<RectPlane<SolidBox>>> GetSurfacesCopy() override;
	static void Load(std::vector<std::string>::iterator &itr, const int &vecSize);
	void Save(std::ofstream &outFile) override;
	static void Create();
	static void PrintSolids();
	ConnectionChannel<SolidBox, RectPlane<SolidBox>>* GetConnChannel();
	void Delete();
	std::string GetConnName();

private:
	ConnectionChannel<SolidBox, RectPlane<SolidBox>> m_channel;
	double m_dSideLength;
	static const int m_nNumOfSurfaces = 6;
};

