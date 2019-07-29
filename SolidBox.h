#pragma once

#include "SquarePlane.h"
#include "Shape.h"
#include <string>
#include <set>
#include "ConnectionChannel.h"
#include <map>
#include <afx.h>
#include <memory>

class SolidBox : public CObject, public Shape {

	friend class Utility;

public:
	DECLARE_SERIAL(SolidBox)
	void Serialize(CArchive& ar);

	SolidBox();
	~SolidBox();
	SolidBox(double sideLength);
	double GetSideLength();
	bool GetHasConnection();
	std::string GetShapeName() override;
	ConnectionChannel* GetConnChannel();
	SolidBox& operator=(SolidBox &cube);
	bool operator<(const SolidBox &cube) const;
	bool operator==(const SolidBox &cube) const;
	static std::set<std::string>* GetCubeNames();
	static std::vector<SolidBox> cubeVec;

private:

	double sideLength;
	ConnectionChannel channel;
	bool bHasConnection; // flag for checking if the SolidBox has a connection
	std::string name;
	static int nameIDCounter; // used for naming unique cubes
	static std::set<std::string> cubeNames; // helps enforce uniqueness of cubes created
	static const int planesPerSolidBox;
};

