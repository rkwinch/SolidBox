#pragma once

#include <string>
#include <set>
#include <map>
#include <afx.h>
#include <memory>
#include <vector>
#include "Shape.h"
#include "ConnectionChannel.h"


class SolidBox : public CObject, public Shape {

	friend class Utility;

public:
	DECLARE_SERIAL(SolidBox)
	void Serialize(CArchive& ar);

	SolidBox();
	~SolidBox();
	SolidBox(SolidBox&); 
	SolidBox(double sideLength);
	double GetSideLength();
	bool GetHasConnection();
	std::string GetShapeName() override;
	ConnectionChannel<SolidBox>* GetConnChannel();
	SolidBox& operator=(SolidBox &cube);
	bool operator<(const SolidBox &cube) const;
	bool operator==(const SolidBox &cube) const;
	static std::vector<std::shared_ptr<SolidBox>> cubeVec;
	static int GetPlnsPerSolidBx();
	void Delete();

private:
	void SetName(std::string name);
	double sideLength;
	ConnectionChannel<SolidBox> channel;
	bool bHasConnection; // flag for checking if the SolidBox has a connection
	static int nameIDCounter; // used for naming unique cubes
	static const int planesPerSolidBox;
};

