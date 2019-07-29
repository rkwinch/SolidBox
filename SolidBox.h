#pragma once
#include "stdafx.h"
#include "SquarePlane.h"
#include "Shape.h"
#include "Channel.h"

class SolidBox : public Shape {
	//abstract parent class defines fxns for setting and getting name
public:
	SolidBox(std::vector<SquarePlane*> groupOfPlanes)
	{   
		name = "";
	}
private:
	std::vector<SquarePlane*> groupOfPlanes;
};

