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
#include <algorithm>
#include <vector>

void Utility::Run()
{
	std::string strInput = "";
	char cInput = 0;
	std::cout << "Welcome to SolidBox." << std::endl;

	while (tolower(cInput) != 'q')
	{
		WelcomeAndOptions();
		std::regex acceptableInputExpr("^\\s*([1-6|q|Q])\\s*$"); // looking for 1-6 or q or Q
		                                                         // change to 1-8 when save and load are implemented
		strInput = GetAndValidateInput(acceptableInputExpr);
		cInput = strInput[0]; // making string of length 1 into a single char
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
		/*else if (cInput == '7')
		{
			SaveAllObjects();
		}
		else if (cInput == '8')
		{
			LoadAllObjects();
		}*/
	}
}

void Utility::CreateSolidBox()
{
	std::string strInput = "";
	double dSideLength = 0.0;
	std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
																	// with 0-1 decimals followed by a number (if present) while allowing spaces
	PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "What would you like the length, width, and height to be? (in cm)" << std::endl;
	std::cout << "ex: 4.5" << std::endl;
	strInput = GetAndValidateInput(acceptableInputExpr);
	dSideLength = std::stod(strInput); // converting string input into a double

	while (dSideLength == 0.0) // don't want to make a box with sideLength of 0
	{
		std::cout << "Please input a value that is not 0" << std::endl;
		strInput = GetAndValidateInput(acceptableInputExpr);
		dSideLength = std::stod(strInput);
	}
	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(dSideLength);
	SolidBox::cubeVec.push_back(box);
	PrintNwLnsAndLnDelimiter("-", 55);
}

void Utility::DeleteExistingSolid()
{
	std::string strInput = "";
	PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Type the number corresponding to the cube you wish to delete" << std::endl;
	std::cout << "or press 'b' to go back to the menu." << std::endl;
	std::regex acceptableInputExpr("^\\s*([1-9]+|b|B)\\s*$");
	PrintSolidsInMemory();
	std::cout << std::endl;
	strInput = InputInMapVal(strInput, acceptableInputExpr);

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	auto cubeVecItr = SolidBox::cubeVec.begin() + (stoi(strInput) - 1); // advance iterator by the # given by user 
	
	//erase everything stored associated with cube for deletion
	SolidBox::cubeNames.erase((*cubeVecItr)->name);
	ConnectionChannel::channelNames.erase((*cubeVecItr)->channel.name);

	for (auto plane : (*cubeVecItr)->channel.planeSet)
	{
		SquarePlane::planeNames.erase(plane->name);
	}

	SolidBox::cubeVec.erase(cubeVecItr);
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

	std::string strHeader = "SolidBox name (length of each side in cm)";
	PrintHeader(strHeader);

	auto cubeVecItr = SolidBox::cubeVec.begin(); // USE AUTO HERE??? WHY CANT I MAKE IT WORK THE FIRST TIME???
	for (cubeVecItr; cubeVecItr != SolidBox::cubeVec.end(); ++cubeVecItr)
	{
		std::cout << (*cubeVecItr)->name << " (" << std::fixed << std::setprecision(3) << (*cubeVecItr)->sideLength << ")" << std::endl;
	}

	PrintNwLnsAndLnDelimiter("-", 55);
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

	std::cout << "Enter the number corresponding to the cube you wish to copy." << std::endl;
	std::string strInput = "";
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // looking for the word "cube" followed by
															  // at least one number allowing for leading and trailing spaces
	PrintSolidsInMemory();
	std::cout << std::endl;
	strInput = InputInMapVal(strInput, acceptableInputExpr);

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	auto cubeVecItr = SolidBox::cubeVec.begin();
	cubeVecItr = std::next(cubeVecItr, (stoi(strInput) - 1)); // advance iterator by the # given by user 
	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>((*cubeVecItr)->sideLength);
	SolidBox::cubeVec.push_back(box); 
	PrintNwLnsAndLnDelimiter("-", 55);
}

void Utility::MoveASolid()
{
	std::string strMoveFrom = "";
	std::string strMoveTo = "";

	if (SolidBox::cubeVec.size() < 2)
	{
		std::cout << "There are not enough cubes in memory to make a move." << std::endl;
		return;
	}

	std::cout << "Please enter two numbers corresponding to their cubes separated by the enter key" << std::endl;
	std::cout << "to move one solid to another or press 'b' to go back to main menu." << std::endl;
	std::cout << "(For ex:  1 = 2 moves 2 into 1)" << std::endl;
	PrintSolidsInMemory();
	std::cout << std::endl;
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want two numbers that can be separated by 
															  // spaces

	//------Get cube selections from user----------------
	//moveFrom cube:
	strMoveFrom = InputInMapVal(strMoveFrom, acceptableInputExpr);

	if ((strMoveFrom == "b") | (strMoveFrom == "B")) // user elected to go back to main menu
	{
		return;
	}

	auto cubeVecItr_From = SolidBox::cubeVec.begin();
	cubeVecItr_From = std::next(cubeVecItr_From, (stoi(strMoveFrom) - 1)); // find if name given is the name of a cube made 

	//moveTo cube:
	strMoveTo = InputInMapVal(strMoveTo, acceptableInputExpr);

	if ((strMoveTo == "b") | (strMoveTo == "B")) // user elected to go back to main menu
	{
		return;
	}

	auto cubeVecItr_To = SolidBox::cubeVec.begin();
	cubeVecItr_To = std::next(cubeVecItr_To, (stoi(strMoveTo) - 1)); // find if name given is the name of a cube made 

	if (strMoveFrom == strMoveTo)
	{
		std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
		MoveASolid();
	}

	// It's OK to now move From into To
	**cubeVecItr_To = **cubeVecItr_From; 
}

void Utility::DebugSolidBox()
{
	std::string strInput = "";
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want one numbers that can be surrounded by 
															  // whitespace.
	PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Type the number corresponding to the desired cube for detailed information" << std::endl;
	std::cout << "or press 'b' to go back to the menu.\n" << std::endl;
	PrintSolidsInMemory();
	std::cout << std::endl;
	strInput = InputInMapVal(strInput, acceptableInputExpr);

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	auto cubeVecItr = SolidBox::cubeVec.begin() + (stoi(strInput) - 1); // advance iterator by the # given by user 

	PrintNwLnsAndLnDelimiter("-", 55);
	PrintDebugInfo(cubeVecItr);
	PrintNwLnsAndLnDelimiter("-", 55);
}

void Utility::SaveAllObjects() // not fleshed out.  probably won't implement
{
	auto cubeVecItr = SolidBox::cubeVec.begin(); // USE AUTO HERE??? WHY CANT I MAKE IT WORK THE FIRST TIME???
	for (cubeVecItr; cubeVecItr != SolidBox::cubeVec.end(); ++cubeVecItr)
	{
		std::cout << (*cubeVecItr)->name << " (" << std::fixed << std::setprecision(3) << (*cubeVecItr)->sideLength << ")" << std::endl;
	}

	int nNumber = 6;
	char cLetter = 'a';
	CFile theFile;
	theFile.Open(_T("SolidBox.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive archive(&theFile, CArchive::store);
	archive << nNumber << cLetter;
	archive.Close();
	theFile.Close();
}

void Utility::LoadAllObjects() //not fleshed out.  probably won't implement
{
	int nNumber;
	char cLetter;
	CFile theFile;
	theFile.Open(_T("SolidBox.txt"), CFile::modeRead | CFile::typeBinary);
	CArchive archive(&theFile, CArchive::load);
	archive >> nNumber >> cLetter;
	std::cout << "the values retrieved were:  " << nNumber << " " << cLetter << std::endl;
	archive.Close();
	theFile.Close();
}

//---------------------helper functions below--------------------------------------
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
	//std::cout << "7)  Save" << std::endl;
	//std::cout << "8)  Load" << std::endl;
}

std::string Utility::CreateUniqueName(std::string strNamePrefix, std::set<std::string> strNameSet, int &nameIDCounter)
{
	std::string strName;
	strName = strNamePrefix + std::to_string(nameIDCounter++);
	std::set<std::string>::iterator strNameSetItr = strNameSet.find(strName);

	try //if in set, naming collision has occurred and don't want to construct object
	{
		if (strNameSetItr != strNameSet.end())
		{
			throw std::exception();
		}
	}
	catch (std::exception e)
	{
		std::cout << "Exception:  " << strNamePrefix << " naming collision" << std::endl;
		ShowSolidsInMemory();
	}
	return strName;
}

bool Utility::ValidateInput(std::string strInput, std::regex acceptableInputExpr)
{
	if (strInput.length() == 0)
	{
		return false;
	}

	std::smatch match;

	if (std::regex_match(strInput, match, acceptableInputExpr))
	{
		return true;
	}
	return false;
}

std::string Utility::RemoveSpaces(std::string strInput, std::regex acceptableInputExpr)
{
	strInput = std::regex_replace(strInput, acceptableInputExpr, "$1");
	return strInput;
}

std::string Utility::GetAndValidateInput(std::regex acceptableInputExpr)
{
	bool bIsValid = false;
	std::string strInput = "";

	do
	{
		getline(std::cin, strInput);
		bIsValid = ValidateInput(strInput, acceptableInputExpr);
		if (bIsValid)
		{
			strInput = RemoveSpaces(strInput, acceptableInputExpr);
		}

		if (strInput.length() == 0)
		{
			bIsValid = false;
		}

		if (!bIsValid)
		{
			std::cout << "Please enter valid input  ";
		}
	} while (!bIsValid);

	return strInput;
}

std::string Utility::InputInMapVal(std::string strInput, std::regex acceptableInputExpr)
{
	strInput = GetAndValidateInput(acceptableInputExpr);

	if ((strInput == "b") | (strInput == "B")) // user elected to go back to main menu
	{
		return "b";
	}

	// check if cube exists
	while ((size_t(stoi(strInput)) > SolidBox::cubeVec.size()) || (stoi(strInput) < 1))
	{
		std::cout << "Selection out of bounds.  Please try again or press 'b' to go" << std::endl;
		std::cout << "to the main menu." << std::endl;
		strInput = GetAndValidateInput(acceptableInputExpr);

		if ((strInput == "b") | (strInput == "B")) // user elected to go back to main menu
		{
			return "b";
		}

	}
	return strInput;
}

void Utility::PrintNwLnsAndLnDelimiter(std::string strDelimiter, int nNumOfTimes)
{
	std::cout << "\n";

	for (int ii = 0; ii < nNumOfTimes; ++ii)
	{
		std::cout << strDelimiter;
	}

	std::cout << "\n" << std::endl;
}

void Utility::PrintChar(char cSymbol, int nNumOfTimes)
{
	for (int ii = 0; ii < nNumOfTimes; ++ii)
	{
		std::cout << cSymbol;
	}
}

void Utility::PrintHeader(std::string strHeader)
{
	std::cout << strHeader;
	PrintNwLnsAndLnDelimiter("_", strHeader.length());
}



void Utility::PrintCubeInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
{
	std::string strHeader = "Cube:";
	PrintHeader(strHeader);
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "SolidBox name:" << (*cubeVecItr)->name << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "hasConnection:" << (*cubeVecItr)->GetHasConnection() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "Channel name:" << (*cubeVecItr)->GetConnChannel()->GetConnName() << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "SideLength (cm):" << std::fixed << std::setprecision(3) << (*cubeVecItr)->GetSideLength() << std::endl;
	std::cout << std::endl;
}

void Utility::PrintChannelInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
{
	std::string strHeader = "Channel:";
	PrintHeader(strHeader);
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Channel name:" << (*cubeVecItr)->channel.name << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Associated SolidBox name:" << (*cubeVecItr)->channel.cube->name << std::endl;
	PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Associated planes' names:" << std::endl;

	for (auto planePtr : (*cubeVecItr)->channel.planeSet)
	{
		PrintChar(' ', 32);
		std::cout << std::left << planePtr->GetSqPlaneName() << std::endl;
	}

	std::cout << std::endl;
}

void Utility::PrintPlanesInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
{
	std::string strHeader = "Planes:";
	PrintHeader(strHeader);

	for (auto planePtr : (*cubeVecItr)->channel.planeSet)
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

void Utility::PrintDebugInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
{
	PrintCubeInfo(cubeVecItr);
	PrintChannelInfo(cubeVecItr);
	PrintPlanesInfo(cubeVecItr);
}

void Utility::PrintSolidsInMemory()
{
	int count = 1;
	for (auto strCubeName : SolidBox::cubeNames)
	{
		std::cout << count << ") " << strCubeName << std::endl;
		++count;
	}
}



