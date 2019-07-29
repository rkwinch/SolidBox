#pragma once

#include <string>
#include <set>
#include <map>
#include <memory>
#include <vector>
#include "Shape.h"
#include "ConnectionChannel.h"

class Sphere : public Shape<Sphere> {

	friend class Utility;

public:
	Sphere();
	~Sphere();
	Sphere(Sphere&);
	Sphere(double sideLength);
	double GetRadius();
	static std::vector<std::shared_ptr<Sphere>> sphereVec;
	static int GetSurface();
	void Delete();
	
private:
	double m_dRadius;
	static const int planesPerSphere;
};
