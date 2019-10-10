#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sapi.h>
#include "Utility.h"
#include "Menu.h"
#include "Speech.h"

// command line arg of value 1 means you want voice controls to commands
// command line arg of value != 1 or no value means you want keyboard controls to commands

int main(int argc, char*argv[])
{
	Menu* menu = nullptr;

	if (argc > 1) // passed an argument to control speech recognition
	{
		menu = Menu::GetInstance(argv[1]);
	}
	else
	{
		menu = Menu::GetInstance();
	}

	menu->Run();

	return 0;
}