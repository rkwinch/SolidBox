#include "Utility.h"
#include <iostream>
#include "SolidBox.h"
#include <limits>
#include <string>
#include <regex>
#include <ctype.h>
#include <set>
#include <iomanip>
#include <memory>

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

void Utility::MoveASolid()
{
	
	if (SolidBox::cubeNames.size() < 2)
	{
		std::cout << "There are not enough cubes in memory to make a move." << std::endl;
		return;
	}

	int count = 0;
	std::string moveFrom = "";
	std::string moveTo = "";
	std::cout << "Please two numbers for their corresponding cubes separated by the enter key" << std::endl;
	std::cout << "where the first is the cube you are moving from" << std::endl;
	std::cout << "and the second is the cube you are moving into." << std::endl;
	std::cout << "(For example, 1" << std::endl;
	std::cout << "              3" << std::endl;
	std::cout << "moves the data from the cube in #1 into the cube in #3)\n" << std::endl;

	for (auto cubeName : SolidBox::cubeNames)
	{
		count++;
		std::cout << count << ") " << cubeName << std::endl;
	}
	std::cout << std::endl;
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want two numbers that can be separated by 
	                                                                  // spaces

	//------Get cube selections from user----------------
	//moveFrom cube:
	moveFrom = GetAndValidateInput(acceptableInputExpr);
	std::cout << "moveFrom:" << moveFrom << "." << std::endl;
	if ((moveFrom == "b") | (moveFrom == "B")) // user elected to go back to main menu
	{
		return;
	}
	// check if moveFrom cube exists
	if ((unsigned int(stoi(moveFrom)) > SolidBox::cubeNameAndCubeMap.size()) | (stoi(moveFrom) < 1))
	{
		std::cout << "Selection out of bounds.  Please try again or press 'b' to go" << std::endl;
		std::cout << "to the main menu." << std::endl;
	}
	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr_From = SolidBox::cubeNameAndCubeMap.begin();
	cubeNameAndCubeItr_From = std::next(cubeNameAndCubeItr_From, (stoi(moveFrom) - 1)); // find if name given is the name of a cube made 
	cubeNameAndCubeItr_From = ValAndGetCubeNmItr(cubeNameAndCubeItr_From->first, acceptableInputExpr);
	if (cubeNameAndCubeItr_From == SolidBox::cubeNameAndCubeMap.end()) // user elected to go back to main menu
	{
		return;
	}
	
	//moveTo cube:
	moveTo = GetAndValidateInput(acceptableInputExpr);
	std::cout << "moveTo:" << moveTo << "." << std::endl;
	if ((moveTo == "b") | (moveTo == "B")) // user elected to go back to main menu
	{
		return;
	}
	// check if moveTo cube exists
	if ((unsigned int(stoi(moveTo)) > SolidBox::cubeNameAndCubeMap.size()) | (stoi(moveTo) < 1))
	{
		std::cout << "Selection out of bounds.  Please try again or press 'b' to go" << std::endl;
		std::cout << "to the main menu." << std::endl;
	}
	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr_To = SolidBox::cubeNameAndCubeMap.begin();
	cubeNameAndCubeItr_To = std::next(cubeNameAndCubeItr_To, (stoi(moveTo) - 1)); // find if name given is the name of a cube made 
	cubeNameAndCubeItr_To = ValAndGetCubeNmItr(cubeNameAndCubeItr_To->first, acceptableInputExpr);
	if (cubeNameAndCubeItr_To == SolidBox::cubeNameAndCubeMap.end()) // user elected to go back to main menu
	{
		return;
	}
	//-------End of Get cube selections from user---------------

	//class Resource {

	//public:

	//	Resource& operator=(Resource&& other) {

	//		if (this != &other) {           // If the object isn't being called on itself
	//			delete this->data;          // Delete the object's data
	//			this->data = other.data;    // "Move" other's data into the current object
	//			other.data = nullptr;       // Mark the other object as "empty"
	//		}
	//		return *this;                   // return *this
	//	}

	//	void* data;

	//};
	//std::set<std::string> SquarePlane::planeNames; get rid of this
	//std::set<std::string> SolidBox::cubeNames; keep this
	//std::map <std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>> SolidBox::cubeAndPlanesMap; map to new planes
	//std::map<std::string, double> SolidBox::cubeAndSideLengthMap; map to new length
	//std::map<std::string, std::shared_ptr<SolidBox>> SolidBox::cubeNameAndCubeMap; remove and add again when complete  *****
	//std::set<std::string> ConnectionChannel::channelNames; get rid of and add back ...keep names the same?
	if (moveFrom == moveTo)
	{
		std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
		MoveASolid();
	}
	// It's OK to now move From into To
	std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr_To;
	cubeAndPlanesItr_To = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr_To->second); // now has iterator to 
																					// SolidBox object and its planes
	/*PrintLineDelimiter("-", 55);
	PrintDebugInfo(cubeNameAndCubeItr_To, cubeAndPlanesItr_To);
	PrintLineDelimiter("-", 55);*/

	std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr_From;
	cubeAndPlanesItr_From = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr_From->second); // now has iterator to 
																						  // SolidBox object and its planes
	/*PrintLineDelimiter("-", 55);
	PrintDebugInfo(cubeNameAndCubeItr_From, cubeAndPlanesItr_From);
	PrintLineDelimiter("-", 55);*/

	*(cubeNameAndCubeItr_To->second) = *(cubeNameAndCubeItr_From->second);

	//cubeAndPlanesItr_To = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr_To->second); // now has iterator to 
	//																				// SolidBox object and its planes
	//PrintLineDelimiter("-", 55);
	//PrintDebugInfo(cubeNameAndCubeItr_To, cubeAndPlanesItr_To);
	//PrintLineDelimiter("-", 55);

	//cubeAndPlanesItr_From = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr_From->second); // now has iterator to 
	//																					  // SolidBox object and its planes
	//PrintLineDelimiter("-", 55);
	//PrintDebugInfo(cubeNameAndCubeItr_From, cubeAndPlanesItr_From);
	//PrintLineDelimiter("-", 55);
}

void Utility::DeleteExistingSolid()
{
	std::string input = "";
	std::cout << "Type the name of the cube you wish to delete" << std::endl;
	std::cout << "or press 'b' to go back to the menu." << std::endl;
	std::regex acceptableInputExpr("^\\s*((cube[0-9]+)|b|B)\\s*$");
	input = GetAndValidateInput(acceptableInputExpr);
	if ((input == "b") || (input == "B"))
	{
		return;
	}
	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr;
	cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(input);
	while (cubeNameAndCubeItr == SolidBox::cubeNameAndCubeMap.end())
	{
		std::cout << "Solid not found.  Please try again or press 'b' to go" << std::endl;
		std::cout << "back to the menu." << std::endl;
		input = GetAndValidateInput(acceptableInputExpr);
		if ((input == "b") || (input == "B"))
		{
			return;
		}
		cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(input);
	}
	SolidBox::cubeNames.erase(input);
	ConnectionChannel::channelNames.erase(cubeNameAndCubeItr->second->channel->name);
	SolidBox::cubeAndSideLengthMap.erase(cubeNameAndCubeItr->first);
	std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr;
	cubeAndPlanesItr = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr->second);
	for (auto plane : cubeAndPlanesItr->second)
	{
		SquarePlane::planeNames.erase(plane->name);
	}
	SolidBox::cubeAndPlanesMap.erase(cubeAndPlanesItr->first);
	SolidBox::cubeNameAndCubeMap.erase(input);

	            //std::set<std::string> SquarePlane::planeNames;
	            //std::set<std::string> SolidBox::cubeNames;
	            /*std::map <std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>> SolidBox::cubeAndPlanesMap;
	            std::map<std::string, double> SolidBox::cubeAndSideLengthMap;
	            std::map<std::string, std::shared_ptr<SolidBox>> SolidBox::cubeNameAndCubeMap;*/
	            //std::set<std::string> ConnectionChannel::channelNames;
}

void Utility::CreateSolidBox()
{
	std::string input = "";
	double sideLength = 0.0;
	std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
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
	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(sideLength);
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
	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr;
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
	std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr;
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
		else if (cInput == '2')
		{
			DeleteExistingSolid();
		}
		else if (cInput == '3')
		{
			ShowSolidsInMemory();
		}
		else if (cInput == '4')
		{
			CopyExistingSolid();
		}
		else if (cInput == '5')
		{
			MoveASolid();
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

void Utility::PrintDebugInfo(std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr, std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr)
{
	PrintCubeInfo(cubeNameAndCubeItr, cubeAndPlanesItr);
	PrintChannelInfo(cubeAndPlanesItr);
	PrintPlanesInfo(cubeAndPlanesItr);	
}

void Utility::PrintCubeInfo(std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr, std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr)
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

void Utility::PrintChannelInfo(std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr)
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

void Utility::PrintPlanesInfo(std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr)
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
	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr;
	cubeNameAndCubeItr = ValAndGetCubeNmItr(input, acceptableInputExpr);
	if (cubeNameAndCubeItr == SolidBox::cubeNameAndCubeMap.end()) // user elected to go back to main menu
	{
		return;
	}
	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(cubeNameAndCubeItr->second->GetSideLength());
	SolidBox::AddCubeAndPlanesToMap(box);
	SolidBox::AddCubeNameAndCubeToMap(box);
	PrintLineDelimiter("-", 55);
}

std::map<std::string, std::shared_ptr<SolidBox>>::iterator Utility::ValAndGetCubeNmItr(std::string input, std::regex acceptableInputExpr)
{
	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr;
	cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(input); // find if name given is the name of a cube made
	while (cubeNameAndCubeItr == SolidBox::cubeNameAndCubeMap.end())
	{
		std::cout << input << " not found" << std::endl;
		std::cout << "Please try again or press 'b' to go to main menu." << std::endl;
		input = GetAndValidateInput(acceptableInputExpr);
		if ((input == "b") || (input == "B"))
		{
			return SolidBox::cubeNameAndCubeMap.end();
		}
		cubeNameAndCubeItr = SolidBox::cubeNameAndCubeMap.find(input);
	}
	return cubeNameAndCubeItr;
}
