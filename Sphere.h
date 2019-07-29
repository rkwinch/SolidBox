#pragma once

#include <string>
#include <set>
#include <map>
#include <afx.h>
#include <memory>
#include <vector>
#include "Shape.h"
#include "ConnectionChannel.h"

class Sphere : public CObject, public Shape{

public:
	DECLARE_SERIAL(Sphere)
	void Serialize(CArchive& ar);

	Sphere();
	~Sphere();
	Sphere(Sphere&);
	Sphere(double radius);
	double GetRadius();
	bool GetHasConnection();
	std::string GetShapeName() override;
	ConnectionChannel<Sphere>* GetConnChannel();
	Sphere& operator=(Sphere &sphere);
	bool operator<(const Sphere &sphere) const;
	bool operator==(const Sphere &sphere) const;
	static std::vector<std::shared_ptr<Sphere>> sphereVec;
	void Delete();

private:
	void SetName(std::string name);
	double radius;
	ConnectionChannel<Sphere> channel;
	bool bHasConnection;
	static int nameIDCounter;
};






