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
#include <afx.h>
#include <afxwin.h>

void Utility::MoveASolid()
{
	std::string moveFrom = "";
	std::string moveTo = "";

	if (SolidBox::cubeVec.size() < 2)
	{
		std::cout << "There are not enough cubes in memory to make a move." << std::endl;
		return;
	}

	std::cout << "Please enter two numbers corresponding to their cubes separated by the enter key" << std::endl;
	std::cout << "to move one solid to another." << std::endl;
	std::cout << "(For ex:  1 = 2 moves 2 into 1)" << std::endl;
	PrintSolidsInMemory();
	std::cout << std::endl;
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want two numbers that can be separated by 
															  // spaces

	//------Get cube selections from user----------------
	//moveFrom cube:
	moveFrom = InputInMapVal(moveFrom, acceptableInputExpr);

	if ((moveFrom == "b") | (moveFrom == "B")) // user elected to go back to main menu
	{
		return;
	}

	auto cubeVecItr_From = SolidBox::cubeVec.begin();
	cubeVecItr_From = std::next(cubeVecItr_From, (stoi(moveFrom) - 1)); // find if name given is the name of a cube made 

	//moveTo cube:
	moveTo = InputInMapVal(moveTo, acceptableInputExpr);

	if ((moveTo == "b") | (moveTo == "B")) // user elected to go back to main menu
	{
		return;
	}

	auto cubeVecItr_To = SolidBox::cubeVec.begin();
	cubeVecItr_To = std::next(cubeVecItr_To, (stoi(moveTo) - 1)); // find if name given is the name of a cube made 

	if (moveFrom == moveTo)
	{
		std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
		MoveASolid();
	}

	// It's OK to now move From into To
	*cubeVecItr_To = *cubeVecItr_From; // to call copy constructor of SolidBox
}

void Utility::DeleteExistingSolid()
{
	std::string input = "";
	PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Type the number corresponding to the cube you wish to delete" << std::endl;
	std::cout << "or press 'b' to go back to the menu." << std::endl;
	std::regex acceptableInputExpr("^\\s*((cube[0-9]+)|b|B)\\s*$");
	PrintSolidsInMemory();
	std::cout << std::endl;
	input = InputInMapVal(input, acceptableInputExpr);

	if ((input == "b") || (input == "B"))
	{
		return;
	}

	auto cubeVecItr = SolidBox::cubeVec.begin();
	cubeVecItr = std::next(cubeVecItr, (stoi(input) - 1)); // advance iterator by the # given by user 
	
																	   //erase everything stored associated with cube for deletion
	SolidBox::cubeNames.erase(cubeVecItr->name);
	ConnectionChannel::channelNames.erase(cubeVecItr->channel.name);

	for (auto plane : cubeVecItr->channel.planeSet)
	{
		SquarePlane::planeNames.erase(plane->name);
	}

	SolidBox::cubeVec.erase(cubeVecItr);
}

void Utility::CreateSolidBox()
{
	std::string input = "";
	double sideLength = 0.0;
	std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
																	// with 0-1 decimals followed by a number (if present) while allowing spaces
	PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "What would you like the length, width, and height to be? (in cm)" << std::endl;
	std::cout << "ex: 4.5" << std::endl;
	input = GetAndValidateInput(acceptableInputExpr);
	sideLength = std::stod(input); // converting string input into a double

	while (sideLength == 0.0) // don't want to make a box with sideLength of 0
	{
		input = GetAndValidateInput(acceptableInputExpr);
		sideLength = std::stod(input);
	}

	SolidBox::cubeVec.emplace_back(SolidBox(sideLength)); // making the object directly into the vector
	PrintNwLnsAndLnDelimiter("-", 55);
}

void Utility::DebugSolidBox()
{
	std::string input = "";
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want one numbers that can be surrounded by 
															  // whitespace.
	PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Type the number corresponding to the desired cube for detailed information" << std::endl;
	std::cout << "or press 'b' to go back to the menu.\n" << std::endl;
	PrintSolidsInMemory();
	std::cout << std::endl;
	input = InputInMapVal(input, acceptableInputExpr);

	if ((input == "b") || (input == "B"))
	{
		return;
	}

	auto cubeVecItr = SolidBox::cubeVec.begin();
	cubeVecItr = std::next(cubeVecItr, (stoi(input) - 1)); // advance iterator by the # given by user 

	PrintNwLnsAndLnDelimiter("-", 55);
	PrintDebugInfo(cubeVecItr);
	PrintNwLnsAndLnDelimiter("-", 55);
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
		else if (cInput == '7')
		{
			SaveAllObjects();
		}
		else if (cInput == '8')
		{
			LoadAllObjects();
		}
	}
}

void Utility::LoadAllObjects() //not fleshed out.  probably won't implement
{
	int x; 
	char c;
	CFile theFile;
	theFile.Open(_T("SolidBox.txt"), CFile::modeRead | CFile::typeBinary);
	CArchive archive(&theFile, CArchive::load);
	archive >> x >> c;
	std::cout << "the values retrieved were:  " << x << " " << c << std::endl;
	archive.Close();
	theFile.Close();
}

void Utility::SaveAllObjects() // not fleshed out.  probably won't implement
{
	auto cubeVecItr = SolidBox::cubeVec.begin(); // USE AUTO HERE??? WHY CANT I MAKE IT WORK THE FIRST TIME???
	for (cubeVecItr; cubeVecItr != SolidBox::cubeVec.end(); ++cubeVecItr)
	{
		std::cout << cubeVecItr->name << " (" << std::fixed << std::setprecision(3) << cubeVecItr->sideLength << ")" << std::endl;
	}
	
	int x = 6;
	char c = 'a';
	CFile theFile;
	theFile.Open(_T("SolidBox.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive archive(&theFile, CArchive::store);
	archive << x << c;
	archive.Close();
	theFile.Close();
}

std::string Utility::GetAndValidateInput(std::regex acceptableInputExpr)
{
	bool bIsValid = false;
	std::string input = "";

	do
	{
		getline(std::cin, input);
		bIsValid = ValidateInput(input, acceptableInputExpr);
		if (bIsValid)
		{
			input = RemoveSpaces(input, acceptableInputExpr);
		}

		if (input.length() == 0)
		{
			bIsValid = false;
		}

		if (!bIsValid)
		{
			std::cout << "Please enter valid input  ";
		}
	} while (!bIsValid);

	return input;
}

std::string Utility::RemoveSpaces(std::string input, std::regex acceptableInputExpr)
{
	input = std::regex_replace(input, acceptableInputExpr, "$1");
	return input;
}

void Utility::ShowSolidsInMemory()
{
	PrintNwLnsAndLnDelimiter("-", 55);

	if (SolidBox::GetCubeNames()->size() == 0)
	{
		std::cout << "No solids currently in memory" << std::endl;
		PrintNwLnsAndLnDelimiter("-", 55);
		return;
	}

	std::string header = "SolidBox name (length of each side in cm)";
	PrintHeader(header);
	
	auto cubeVecItr = SolidBox::cubeVec.begin(); // USE AUTO HERE??? WHY CANT I MAKE IT WORK THE FIRST TIME???
	for (cubeVecItr; cubeVecItr != SolidBox::cubeVec.end(); ++cubeVecItr)
	{
		std::cout << cubeVecItr->name << " (" << std::fixed << std::setprecision(3) << cubeVecItr->sideLength << ")" << std::endl;
	}

	PrintNwLnsAndLnDelimiter("-", 55);
}

void Utility::PrintNwLnsAndLnDelimiter(std::string delimiter, int numOfTimes)
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
	PrintNwLnsAndLnDelimiter("_", header.length());
}

void Utility::PrintDebugInfo(std::vector<SolidBox>::iterator cubeVecItr)
{
	PrintCubeInfo(cubeVecItr);
	PrintChannelInfo(cubeVecItr);
	PrintPlanesInfo(cubeVecItr);
}

void Utility::PrintCubeInfo(std::vector<SolidBox>::iterator cubeVecItr)
{
	std::string header = "Cube:";
	PrintHeader(header);
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "SolidBox name:" << cubeVecItr->name << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "hasConnection:" << cubeVecItr->GetHasConnection() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "Channel name:" << cubeVecItr->GetConnChannel()->GetConnName() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "SideLength (cm):" << std::fixed << std::setprecision(3) << cubeVecItr->GetSideLength() << std::endl;
	std::cout << std::endl;
}

void Utility::PrintChannelInfo(std::vector<SolidBox>::iterator cubeVecItr)
{
	std::string header = "Channel:";
	PrintHeader(header);
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Channel name:" << cubeVecItr->channel.name << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Associated SolidBox name:" << cubeVecItr->channel.GetSolidBox()->GetShapeName() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Associated planes' names:" << std::endl;

	for (auto planePtr : cubeVecItr->channel.planeSet)
	{
		PrintChar(' ', 32);
		std::cout << std::left << planePtr->GetSqPlaneName() << std::endl;
	}

	std::cout << std::endl;
}

void Utility::PrintPlanesInfo(std::vector<SolidBox>::iterator cubeVecItr)
{
	std::string header = "Planes:";
	PrintHeader(header);

	for (auto planePtr : cubeVecItr->channel.planeSet)
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
	PrintNwLnsAndLnDelimiter("-", 55);

	if (SolidBox::GetCubeNames()->size() == 0)
	{
		std::cout << "No solids currently in memory" << std::endl;
		PrintNwLnsAndLnDelimiter("-", 55);
		return;
	}

	std::cout << "Enter the name of the cube you wish to copy." << std::endl;
	std::string input = "";
	std::regex acceptableInputExpr("^\\s*((cube[0-9]+)|b|B)\\s*$"); // looking for the word "cube" followed by
																	// at least one number allowing for leading and trailing spaces
	PrintSolidsInMemory();
	std::cout << std::endl;
	input = InputInMapVal(input, acceptableInputExpr);

	if ((input == "b") || (input == "B"))
	{
		return;
	}

	auto cubeVecItr = SolidBox::cubeVec.begin();
	cubeVecItr = std::next(cubeVecItr, (stoi(input) - 1)); // advance iterator by the # given by user 

	SolidBox::cubeVec.emplace_back(SolidBox(cubeVecItr->sideLength)); // make the copy inside the vector
	PrintNwLnsAndLnDelimiter("-", 55);
}

std::string Utility::InputInMapVal(std::string input, std::regex acceptableInputExpr)
{
	input = GetAndValidateInput(acceptableInputExpr);

	if ((input == "b") | (input == "B")) // user elected to go back to main menu
	{
		return "b";
	}

	// check if cube exists
	while ((size_t(stoi(input)) > SolidBox::cubeVec.size()) || (stoi(input) < 1))
	{
		std::cout << "Selection out of bounds.  Please try again or press 'b' to go" << std::endl;
		std::cout << "to the main menu." << std::endl;
		input = GetAndValidateInput(acceptableInputExpr);

		if ((input == "b") | (input == "B")) // user elected to go back to main menu
		{
			return "b";
		}

	}
	return input;
}

void Utility::PrintSolidsInMemory()
{
	int count = 1;
	for (auto cubeName : SolidBox::cubeNames)
	{
		std::cout << count << ") " << cubeName << std::endl;
		++count;
	}
}