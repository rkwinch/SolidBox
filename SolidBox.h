#pragma once

#include <string>
#include <set>
#include <map>
#include <memory>
#include <vector>
#include "Shape.h"
#include "ConnectionChannel.h"
#include "RectPlane.h"

class SolidBox : public Shape<SolidBox, RectPlane<SolidBox>> {

	friend class Utility;

public:
	SolidBox();
	~SolidBox();
	SolidBox(SolidBox&);
	SolidBox(double sideLength);
	double GetSideLength();
	SolidBox& operator=(SolidBox &cube);
	static std::vector<std::shared_ptr<SolidBox>> cubeVec;
	static int GetPlnsPerSolidBx();
	void Delete();
	static void LoadSolidBox(std::vector<std::string>::iterator &itr, const int &vecSize);
	void Save(std::ofstream &outFile);
	std::set<std::shared_ptr<RectPlane<SolidBox>>> GetPlanesCopy();

private:
	double m_dSideLength;
	ConnectionChannel<SolidBox, RectPlane<SolidBox>> m_channel;
	bool bHasConnection; // flag for checking if the SolidBox has a connection
	static int nameIDCounter; // used for naming unique cubes
	static const int planesPerSolidBox;
};

