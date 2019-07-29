#include "Utility.h"
#include <iostream>
#include "Solidbox.h"
#include <limits>
#include <string>
#include <regex>

//solid is moved to other solid the previous solid should be removed from the memory.
//
//Operations(d) and (e)above do not include the name of the object.Name remains a unique identifier.The name is your only way of  identifying an object.
//
//f)	Debug a solid(print information about a particular solid - includes going down to planes using the channel connection).
//
//g)	Save the current set of objects to a disk file using the Serialization protocol provided by the Microsoft Class - CObject.
//
//h)	Read an existing disk file into memory - reverse of(g).
//
//The implementation should be such that the planes are not directly accessed after construction.They should be accessed only through the solid object via the channel.

void Utility::CreateSolidBox()
{
	std::string input = "";
	double sideLength = 0.0;
	bool isValid = false;
	while(!isValid)
	{
		std::cout << "What would you like the length, width, and height to be? (in cm)" << std::endl;
		std::cout << "ex: 4.5" << std::endl;
		getline(std::cin, input);
		isValid = ValidateSideLengthInput(input);
	} 
	sideLength = std::stod(input);
	SolidBox box(sideLength);
}

bool Utility::ValidateSideLengthInput(std::string input)
{
	int validInputCounter = 0;
	int decimalCounter = 0;
	int zeroCounter = 0;
	for (int ii = 0; ii < input.length(); ++ii)
	{
		if (input[ii] == '.')
		{
			++decimalCounter;
		}

		if (input[ii] == '0')
		{
			++zeroCounter;
		}

		if (((input[ii] != '.') && (!isdigit(input[ii]))) || (decimalCounter > 1))
		{
			std::cin.clear();
			std::cout << "Invalid input.  Please try again." << std::endl;
			return false;
		}
		++validInputCounter;
	}

	if ((zeroCounter == input.length()) || ((zeroCounter == input.length() - 1) && (decimalCounter == 1)) || validInputCounter != input.size() || (decimalCounter == validInputCounter))
	{
		std::cin.clear();
		std::cout << "Invalid input.  Please try again." << std::endl;
		return false;
	}
	return true;
}

bool Utility::ValidateMenuInput(std::string input)
{
	std::regex acceptableInputExpr([1-8] or q or Q);
	if (input.length() != 1)
	{
		std::cin.clear();
		std::cout << "Invalid input.  Please try again." << std::endl;
		return false;
	}
	return true;
}

void Utility::WelcomeAndOptions()
{
	std::cout << "Welcome to Solidbox.  What would you like to do?" << std::endl;
	std::cout << "(enter a number or press 'q' to quit)\n\n" << std::endl;
	std::cout << "1)  Create a solid box" << std::endl;
	std::cout << "2)  Delete a solid box" << std::endl;
	std::cout << "3)  Show available solids" << std::endl;
	std::cout << "4)  Copy an existing solid into a new one" << std::endl;
	std::cout << "5)  Move an existing solid's data into another existing solid" << std::endl;
	std::cout << "6)  Debug a solid (print information)" << std::endl;
	std::cout << "7)  Save" << std::endl;
	std::cout << "8)  Load" << std::endl;
}

void Utility::Run()
{
	bool isValid = false;
	std::string input = "";
	
	while (!isValid || input != "q")
	{
		WelcomeAndOptions();
		getline(std::cin, input);
		//std::cout << "input in run:  " << input << std::endl;
		isValid = ValidateMenuInput(input);
		//write code for mapping fxns(?) to numbers and rest of driver!!
	}
}
