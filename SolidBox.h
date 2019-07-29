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

	SolidBox(double sideLength);
	~SolidBox();
	std::string GetShapeName() override;
	double GetSideLength();
	std::shared_ptr<ConnectionChannel> GetConnChannel();
	bool operator<(const SolidBox &cube) const;
	bool operator==(const SolidBox &cube) const;
	SolidBox& operator=(SolidBox &cube);
	bool GetHasConnection();
	static std::map <std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>> cubeAndPlanesMap;
	static std::set<std::string>* GetCubeNames();
	static std::map<std::string, double> cubeAndSideLengthMap;
	static std::map<std::string, std::shared_ptr<SolidBox>> cubeNameAndCubeMap;
	static void AddCubeAndPlanesToMap(std::shared_ptr<SolidBox> cube);
	static void AddCubeNameAndCubeToMap(std::shared_ptr<SolidBox> cube);


private:
	
	double sideLength;
	std::shared_ptr<ConnectionChannel> channel;
	bool hasConnection; // flag for checking if the SolidBox has a connection
	std::string name;
	static int nameIDCounter; // used for naming unique cubes
	static std::set<std::string> cubeNames; // helps enforce uniqueness of cubes created
	static const int planesPerSolidBox;
	static std::set<SolidBox> cubes;

};
