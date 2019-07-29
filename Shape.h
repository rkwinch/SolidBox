#pragma once
#include "stdafx.h"
#include <vector>
#include "SquarePlane.h"
#include <string>

//abstract.  Don't make instances of Shape.
class Shape {
protected:
	
	std::string name;
public:
	std::string GetShapeName()
	{
		return name;
	}
	void SetShapeName(std::string name)
	{
		this->name = name;
	}
};