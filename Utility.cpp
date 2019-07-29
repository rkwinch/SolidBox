#include "Utility.h"
#include <iostream>
#include "SolidBox.h"
#include <limits>
#include <string>
#include <regex>
#include <ctype.h>
#include <set>
#include <iomanip>

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

	while (!isValid)
	{
		std::cout << "What would you like the length, width, and height to be? (in cm)" << std::endl;
		std::cout << "ex: 4.5" << std::endl;
		getline(std::cin, input);
		isValid = ValidateSideLengthInput(input);
	}
	sideLength = std::stod(input);
	SolidBox box(sideLength);
	SolidBox::AddCubeAndPlanesToMap(&box); // problem here?
	SolidBox::AddCubeNameAndCubeToMap(&box); // problem here?
}

void Utility::DebugSolidBox()
{
	/*for (auto mapPair : SolidBox::cubeNameAndCubeMap)
	{
		std::cout << "in for loop for map" << std::endl;
		std::cout << mapPair.first << std::endl;
	}*/
	std::cout << "map size cubename and cube:  " << SolidBox::cubeNameAndCubeMap.size() << std::endl;
	std::cout << "map size cube and planes:  " << SolidBox::cubeAndPlanesMap.size() << std::endl;
	std::cout << "set size cubenames:  " << SolidBox::GetCubeNames()->size() << std::endl;
	std::cout << "map size cube and sidelength:  " << SolidBox::cubeAndSideLengthMap.size() << std::endl;

	//std::cout << "type the name of the cube for detailed information" << std::endl;
	//std::string cubeName = "cube1";
	//std::cin >> cubeName;
	//std::cout << "cubename length:  " << cubeName.length() << std::endl;
	//// get cube belonging to cubeName
	//std::map<std::string, SolidBox*>::iterator cubeNameAndCubeItr;
	//cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(cubeName);
	//try
	//{
	//	if (cubeNameAndCubeItr == SolidBox::cubeNameAndCubeMap.end())
	//	{
	//		throw std::exception();
	//	}
	//}
	//catch (std::exception e)
	//{
	//	std::cout << "Exception:  " << cubeName << " not found" << std::endl;
	//	std::string firstCubeName = SolidBox::cubeNameAndCubeMap.begin()->first;
	//}
	//std::cout << cubeNameAndCubeItr->first << std::endl;
	// get planes and other info belonging to found cube
	std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr;
	//SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr->second);
}

std::string Utility::CreateUniqueName(std::string namePrefix, std::set<std::string> nameSet, int &nameIDCounter)
{
	std::string name;
	name = namePrefix + std::to_string(nameIDCounter++);
	std::set<std::string>::iterator nameSetItr = nameSet.find(name);
	try //if in set, naming collision has occurred and don't want to construct object
	{
		if (nameSetItr != nameSet.end())
		{
			throw std::exception();
		}
	}
	catch (std::exception e)
	{
		std::cout << "Exception:  " << namePrefix << " naming collision" << std::endl;
		SolidBox::PrintCubeSet();
	}
	return name;
}

bool Utility::ValidateSideLengthInput(std::string input) // MAKE SIMPLER LATER!!!!!
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
	if (input.length() == 0)
	{
		return false;
	}
	std::regex acceptableInputExpr("^\\s*([1-8|q|Q])\\s*$"); // looking for 1-8 or q or Q
															 // allowing for leading and trailing whitespaces
	std::smatch match;

	if (std::regex_match(input, match, acceptableInputExpr))
	{
		return true;
	}
	return false;
}

void Utility::WelcomeAndOptions()
{
	std::cout << "What would you like to do?" << std::endl;
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
	char cInput = 0;
	std::cout << "Welcome to SolidBox." << std::endl;
	while (tolower(cInput) != 'q')
	{
		WelcomeAndOptions();
		getline(std::cin, input);
		isValid = ValidateMenuInput(input);
		while (!isValid)
		{
			std::cout << "Please enter valid input  ";
			getline(std::cin, input);
			isValid = ValidateMenuInput(input);
		}
		cInput = MenuInputToChar(input);
		//write code for mapping fxns(?) to numbers and rest of driver!!
		if (cInput == '1')
		{
			CreateSolidBox();
		}
		else if (cInput == '3')
		{
			ShowSolidsInMemory();
		}
		else if (cInput == '6')
		{
			DebugSolidBox();
		}
	}
}

char Utility::MenuInputToChar(std::string input)
{
	std::regex acceptableInputExpr("^\\s*([1-8|q|Q])\\s*$");
	std::string result = std::regex_replace(input, acceptableInputExpr, "$1");
	return result[0];
}

void Utility::ShowSolidsInMemory()
{
	PrintLineDelimiter("-", 55);
	if (SolidBox::GetCubeNames()->size() == 0)
	{
		std::cout << "No solids currently in memory" << std::endl;
		PrintLineDelimiter("-", 55);
		return;
	}
	std::string header = "SolidBox name (length of each side in cm)";	
	std::cout << header;
	PrintLineDelimiter("_", header.length());
	SolidBox::PrintCubeSet();
	std::map<std::string, double>::iterator cubeAndSideLengthItr;
	for (cubeAndSideLengthItr = SolidBox::cubeAndSideLengthMap.begin(); cubeAndSideLengthItr != SolidBox::cubeAndSideLengthMap.end(); ++cubeAndSideLengthItr)
	{
		std::cout << cubeAndSideLengthItr->first << " (" << std::fixed << std::setprecision(3) << cubeAndSideLengthItr->second << ")" << std::endl;
	}
	PrintLineDelimiter("-", 55);
}

void Utility::PrintLineDelimiter(std::string delimiter, int numOfTimes)
{
	std::cout << "\n";
	for (int ii = 0; ii < numOfTimes; ++ii)
	{
		std::cout << delimiter;
	}
	std::cout << "\n" << std::endl;
}
