#pragma once

#include <vector>
#include "SquarePlane.h"
#include <string>

//abstract.  Don't make instances of Shape.
class Shape {

protected:
	std::string name;
	virtual std::string GetShapeName() = 0;
};
