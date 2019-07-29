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
	int count = 0;
	std::string moveFrom = "";
	std::string moveTo = "";

	if (SolidBox::cubeNames.size() < 2)
	{
		std::cout << "There are not enough cubes in memory to make a move." << std::endl;
		return;
	}

	std::cout << "Please enter two numbers corresponding to their cubes separated by the enter key" << std::endl;
	std::cout << "to move one solid to another." << std::endl;
	std::cout << "(For ex:  1 = 2 moves 2 into 1)" << std::endl;

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
	moveFrom = MoveInputVal(moveFrom, acceptableInputExpr);

	if ((moveFrom == "b") | (moveFrom == "B")) // user elected to go back to main menu
	{
		return;
	}

	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr_From = SolidBox::cubeNameAndCubeMap.begin();
	cubeNameAndCubeItr_From = std::next(cubeNameAndCubeItr_From, (stoi(moveFrom) - 1)); // find if name given is the name of a cube made 
	cubeNameAndCubeItr_From = ValAndGetCubeNmItr(cubeNameAndCubeItr_From->first, acceptableInputExpr);

	if (cubeNameAndCubeItr_From == SolidBox::cubeNameAndCubeMap.end()) // user elected to go back to main menu
	{
		return;
	}

	//moveTo cube:
	moveTo = MoveInputVal(moveTo, acceptableInputExpr);

	if ((moveTo == "b") | (moveTo == "B")) // user elected to go back to main menu
	{
		return;
	}

	std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr_To = SolidBox::cubeNameAndCubeMap.begin();
	cubeNameAndCubeItr_To = std::next(cubeNameAndCubeItr_To, (stoi(moveTo) - 1)); // find if name given is the name of a cube made 
	cubeNameAndCubeItr_To = ValAndGetCubeNmItr(cubeNameAndCubeItr_To->first, acceptableInputExpr);

	if (cubeNameAndCubeItr_To == SolidBox::cubeNameAndCubeMap.end()) // user elected to go back to main menu
	{
		return;
	}

	if (moveFrom == moveTo)
	{
		std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
		MoveASolid();
	}

	// It's OK to now move From into To
	std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr_To;
	cubeAndPlanesItr_To = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr_To->second); // now has iterator to 
																						  // SolidBox object and its planes

	std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr_From;
	cubeAndPlanesItr_From = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr_From->second); // now has iterator to 
																							  // SolidBox object and its planes

	*(cubeNameAndCubeItr_To->second) = *(cubeNameAndCubeItr_From->second);
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
	std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr;
	cubeAndPlanesItr = SolidBox::cubeAndPlanesMap.find(cubeNameAndCubeItr->second);

	for (auto plane : cubeAndPlanesItr->second)
	{
		SquarePlane::planeNames.erase(plane->name);
	}

	SolidBox::cubeAndPlanesMap.erase(cubeAndPlanesItr->first);
	SolidBox::cubeNameAndCubeMap.erase(input);

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
	std::string input = "";
	std::regex acceptableInputExpr("^\\s*((cube[0-9]+)|b|B)\\s*$"); // looking for the word "cube" followed by
																	// at least one number allowing for leading and trailing spaces
	std::cout << "Type the name of the cube for detailed information" << std::endl;
	std::cout << "or press 'b' to go back to the menu." << std::endl;
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

void Utility::LoadAllObjects()
{
	int x; char c;
	CFile theFile;
	theFile.Open(_T("SolidBox.txt"), CFile::modeRead | CFile::typeBinary);
	CArchive archive(&theFile, CArchive::load);
	archive >> x >> c;
	std::cout << "the values retrieved were:  " << x << " " << c << std::endl;
	//try
	//{
	//	archive.Read(theFile);
	//	//std::cout << "x after reading in:  " << x << std::endl;
	//}
	//catch (CException* e)
	//{
	//	TCHAR   szCause[255];
	//	CString strFormatted;
	//	std::cout << "in catch block" << std::endl;

	//	e->GetErrorMessage(szCause, 255);
	//	strFormatted += szCause;
	//	AfxMessageBox(strFormatted);
	//}

	archive.Close();
	theFile.Close();
}

void Utility::SaveAllObjects()
{
	for (auto element : SolidBox::cubeAndPlanesMap)
	{
		std::cout << element.first->name << std::endl;
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
	auto cubeAndPlanesItr = SolidBox::cubeAndPlanesMap.begin();

	for (cubeAndPlanesItr; cubeAndPlanesItr != SolidBox::cubeAndPlanesMap.end(); ++cubeAndPlanesItr)
	{
		std::cout << cubeAndPlanesItr->first->GetShapeName() << " (" << std::fixed << std::setprecision(3) << cubeAndPlanesItr->first->GetSideLength() << ")" << std::endl;
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
	std::cout << "in valandgetcubenmitr" << std::endl;

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

std::string Utility::MoveInputVal(std::string input, std::regex acceptableInputExpr)
{
	input = GetAndValidateInput(acceptableInputExpr);

	if ((input == "b") | (input == "B")) // user elected to go back to main menu
	{
		return "b";
	}

	// check if moveFrom cube exists
	while ((unsigned int(stoi(input)) > SolidBox::cubeNameAndCubeMap.size()) | (stoi(input) < 1))
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
