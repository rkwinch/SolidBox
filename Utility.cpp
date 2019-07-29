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
	std::regex acceptableInputExpr("^\\s*([0-9]*\.?[0-9]*)\\s*$"); // looking for a number (if present)
																   // with 0-1 decimals followed by a number (if present) while allowing spaces
	std::cout << "What would you like the length, width, and height to be? (in cm)" << std::endl;
	std::cout << "ex: 4.5" << std::endl;
	input = GetAndValidateInput(acceptableInputExpr);
	sideLength = std::stod(input); // converting string input into a double
	while (sideLength == 0.0) // don't want to make a box with sideLength of 0
	{
		input = GetAndValidateInput(acceptableInputExpr);
		sideLength = std::stod(input);
	}
	SolidBox* box = new SolidBox(sideLength);
	SolidBox::AddCubeAndPlanesToMap(box); 
	SolidBox::AddCubeNameAndCubeToMap(box); 
}

void Utility::DebugSolidBox()
{
	
	std::cout << "Type the name of the cube for detailed information" << std::endl;
	std::cout << "or press 'b' to go back to the menu." << std::endl;
	std::string input = "";
	std::regex acceptableInputExpr("^\\s*((cube[0-9]+)|b|B)\\s*$"); // looking for the word "cube" followed by
	                    // at least one number allowing for leading and trailing spaces
	input = GetAndValidateInput(acceptableInputExpr);
	if ((input == "b") || (input == "B"))
	{
		return;
	}
	// get cube belonging to cubeName
	std::map<std::string, SolidBox*>::iterator cubeNameAndCubeItr;
	cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(input); // find if name given is the name of a cube made
	while (cubeNameAndCubeItr == SolidBox::cubeNameAndCubeMap.end())
	{
		std::cout << input << " not found" << std::endl;
		std::cout << "Please try again or press 'b' to go to main menu." << std::endl;
		input = GetAndValidateInput(acceptableInputExpr);
		if ((input == "b") || (input == "B"))
		{
			return;
		}
		cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(input);
	}
	std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr;
	cubeAndPlanesItr = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr->second); // now has iterator to 
	                                                // SolidBox object and its planes
	PrintLineDelimiter("-", 55);
	PrintDebugInfo(cubeNameAndCubeItr, cubeAndPlanesItr);
	PrintLineDelimiter("-", 55);
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
		ShowSolidsInMemory();
	}
	return name;
}

bool Utility::ValidateInput(std::string input, std::regex acceptableInputExpr)
{
	if (input.length() == 0)
	{
		return false;
	}
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
	std::string input = "";
	char cInput = 0;
	std::cout << "Welcome to SolidBox." << std::endl;
	while (tolower(cInput) != 'q')
	{
		WelcomeAndOptions();
		std::regex acceptableInputExpr("^\\s*([1-8|q|Q])\\s*$"); // looking for 1-8 or q or Q
		input = GetAndValidateInput(acceptableInputExpr);
		cInput = input[0]; // making string of length 1 into a single char
		//write code for mapping fxns(?) to numbers and rest of driver!!
		if (cInput == '1')
		{
			CreateSolidBox();
		}
		else if (cInput == '3')
		{
			ShowSolidsInMemory();
		}
		else if (cInput == '4')
		{
			CopyExistingSolid();
		}
		else if (cInput == '6')
		{
			DebugSolidBox();
		}
	}
}

std::string Utility::GetAndValidateInput(std::regex acceptableInputExpr)
{
	bool isValid = false;
	std::string input = "";
	do
	{
		getline(std::cin, input);
		isValid = ValidateInput(input, acceptableInputExpr);
		if (isValid)
		{
			input = RemoveSpaces(input, acceptableInputExpr);
		}

		if (input.length() == 0)
		{
			isValid = false;
		}

		if (!isValid)
		{
			std::cout << "Please enter valid input  ";
		}
	} while (!isValid);
	return input;
}

std::string Utility::RemoveSpaces(std::string input, std::regex acceptableInputExpr)
{
	input = std::regex_replace(input, acceptableInputExpr, "$1");
	return input;
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
	PrintHeader(header);
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

void Utility::PrintChar(char symbol, int numOfTimes)
{
	for (int ii = 0; ii < numOfTimes; ++ii)
	{
		std::cout << symbol;
	}
}

void Utility::PrintHeader(std::string header)
{
	std::cout << header;
	PrintLineDelimiter("_", header.length());
}

void Utility::PrintDebugInfo(std::map<std::string, SolidBox*>::iterator cubeNameAndCubeItr, std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr)
{
	PrintCubeInfo(cubeNameAndCubeItr, cubeAndPlanesItr);
	PrintChannelInfo(cubeAndPlanesItr);
	PrintPlanesInfo(cubeAndPlanesItr);	
}

void Utility::PrintCubeInfo(std::map<std::string, SolidBox*>::iterator cubeNameAndCubeItr, std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr)
{
	std::string header = "Cube:";
	PrintHeader(header);
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "SolidBox name:" << cubeNameAndCubeItr->first << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "hasConnection:" << cubeAndPlanesItr->first->GetHasConnection() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "Channel name:" << cubeAndPlanesItr->first->GetConnChannel()->GetConnName() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "SideLength (cm):" << std::fixed << std::setprecision(3) << cubeAndPlanesItr->first->GetSideLength() << std::endl;
	std::cout << std::endl;
}

void Utility::PrintChannelInfo(std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr)
{
	std::string header = "Channel:";
	PrintHeader(header);
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Channel name:" << cubeAndPlanesItr->first->GetConnChannel()->GetConnName() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Associated SolidBox name:" << cubeAndPlanesItr->first->GetConnChannel()->GetSolidBox()->GetShapeName() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Associated planes' names:" << std::endl;
	for (auto planePtr : cubeAndPlanesItr->second)
	{
		PrintChar(' ', 32);
		std::cout << std::left << planePtr->GetSqPlaneName() << std::endl;
	}
	std::cout << std::endl;
}

void Utility::PrintPlanesInfo(std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr)
{
	std::string header = "Planes:";
	PrintHeader(header);
	for (auto planePtr : cubeAndPlanesItr->second)
	{
		PrintChar(' ', 5);
		std::cout << std::left << std::setw(26) << "Plane name:" << planePtr->GetSqPlaneName() << std::endl;
		PrintChar(' ', 5);
		std::cout << std::left << std::setw(26) << "Associated channel name:" << planePtr->GetConnChannel()->GetConnName() << std::endl;
		PrintChar(' ', 5);
		std::cout << std::left << std::setw(26) << "Number of edges:" << planePtr->GetNumOfEdges() << std::endl;
		PrintChar(' ', 5);
		std::cout << std::left << std::setw(26) << "Length:" << std::fixed << std::setprecision(3) << planePtr->GetSqPlaneLength() << std::endl;
		PrintChar(' ', 5);
		std::cout << std::left << std::setw(26) << "Height:" << std::fixed << std::setprecision(3) << planePtr->GetSqPlaneHeight() << std::endl;
		std::cout << std::endl;
	}
}

void Utility::CopyExistingSolid()
{
	PrintLineDelimiter("-", 55);
	if (SolidBox::GetCubeNames()->size() == 0)
	{
		std::cout << "No solids currently in memory" << std::endl;
		PrintLineDelimiter("-", 55);
		return;
	}
	std::cout << "Enter the name of the cube you wish to copy." << std::endl;
	std::string input = "";
	std::regex acceptableInputExpr("^\\s*((cube[0-9]+)|b|B)\\s*$"); // looking for the word "cube" followed by
																	// at least one number allowing for leading and trailing spaces
	input = GetAndValidateInput(acceptableInputExpr);
	if ((input == "b") || (input == "B"))
	{
		return;
	}
	// get cube belonging to cubeName
	std::map<std::string, SolidBox*>::iterator cubeNameAndCubeItr;
	cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(input); // find if name given is the name of a cube made
	while (cubeNameAndCubeItr == SolidBox::cubeNameAndCubeMap.end())
	{
		std::cout << input << " not found" << std::endl;
		std::cout << "Please try again or press 'b' to go to main menu." << std::endl;
		input = GetAndValidateInput(acceptableInputExpr);
		if ((input == "b") || (input == "B"))
		{
			return;
		}
		cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(input);
	}
	SolidBox* box = new SolidBox(cubeNameAndCubeItr->second->GetSideLength());
	SolidBox::AddCubeAndPlanesToMap(box);
	SolidBox::AddCubeNameAndCubeToMap(box);
	PrintLineDelimiter("-", 55);
}
