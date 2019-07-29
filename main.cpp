#include <iostream>
#include "Solidbox.h"
#include "SquarePlane.h"

int main()
{
	std::vector<SquarePlane*> groupOfPlanes;
	std::cout << "this works..." << std::endl;
	SolidBox box1(4);
	SolidBox box2(6);
	SolidBox box3(3);
	SolidBox::PrintCubeSet();

	system("pause");
	return 0;
}