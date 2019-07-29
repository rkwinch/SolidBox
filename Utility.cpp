#include <iostream>
#include <limits>
#include <string>
#include <regex>
#include <ctype.h>
#include <set>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <vector>
#include <fstream>
#include <Windows.h>
#include "ConnectionChannel.h"
#include "SquarePlane.h"
#include "SolidBox.h"
#include "Utility.h"

//initializing static member variable
int ConnectionChannel<SolidBox>::nameIDCounter = 0;

void Utility::Run()
{
	std::string strInput = "";
	char cInput = 0;
	std::cout << "Welcome to SolidBox." << std::endl;

	while (tolower(cInput) != 'q')
	{
		WelcomeAndOptions();
		std::regex acceptableInputExpr("^\\s*([1-8|q|Q])\\s*$"); // looking for 1-8 or q or Q                                                         
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
	(**cubeVecItr).Delete(); // dereference twice (first for iterator, second for shared ptr)
}

void Utility::ShowSolidsInMemory()
{
	PrintNwLnsAndLnDelimiter("-", 55);

	if (SolidBox::cubeVec.size() == 0)
	{
		std::cout << "No solids currently in memory" << std::endl;
		PrintNwLnsAndLnDelimiter("-", 55);
		return;
	}

	std::string strHeader = "SolidBox name (length of each side in cm)";
	PrintHeader(strHeader);


	for (auto cubePtr : SolidBox::cubeVec)
	{
		std::cout << cubePtr->name << " (" << std::fixed << std::setprecision(3) << cubePtr->sideLength << ")" << std::endl;
	}

	PrintNwLnsAndLnDelimiter("-", 55);
}

void Utility::CopyExistingSolid()
{
	PrintNwLnsAndLnDelimiter("-", 55);

	if (SolidBox::cubeVec.size() == 0)
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
	std::cout << std::setw(11) << std::left << "(For ex:  1" << std::endl;
	std::cout << std::setw(11) << std::right << "2" << std::endl;
	std::cout << "moves 2 into 1)" << std::endl;
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
	//check if trying to move to the same cube
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

int Utility::SaveAllObjects()
{
	int count = 0;
	char cInput = 0;
	int numOfFiles = 0;
	std::string fileName = "";
	//if no boxes to save, return and don't save
	if (!IsOkToSave())
	{
		std::cout << "There are no solid boxes to save." << std::endl;
		return 0;
	}

	// view current files in memory and get input from the user to go to main menu,
	// save a new file, or replace an existing file
	ViewFiles();
	numOfFiles = NumOfFilesAvail();
	cInput = SaveOptions();

	if (tolower(cInput) == 'b')
	{
		return 0;
	}
	else if ((tolower(cInput) == 's') && (numOfFiles == 0))
	{
		std::cout << "Cannot pick an existing file.  No files in memory." << std::endl;
		fileName = PickNewFile();
	}
	else if(tolower(cInput) == 's')
	{
		fileName = PickFile();
		if (fileName.length() == 0) // user elected to go back to the main menu
		{
			return 0;
		}
	}
	else if (cInput == 'n')
	{
		fileName = PickNewFile();
	}
	// saving objects to file now
	std::ofstream outFile;
	outFile.open(fileName);
	outFile << SolidBox::nameIDCounter << ";" << ConnectionChannel<SolidBox>::nameIDCounter << ";" << SquarePlane::nameIDCounter << ";";
	outFile << static_cast<int>(SolidBox::cubeVec.size()) << ";\n";
	PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Saving...";

	for (auto cubePtr : SolidBox::cubeVec)
	{
		if (count == 0)
		{
			std::cout << cubePtr->name << " (" << std::fixed << std::setprecision(3) << cubePtr->sideLength << ")" << std::endl;
			++count;
		}
		else
		{
			std::cout << std::setw(9) << "" << std::left << cubePtr->name << " (" << std::fixed << std::setprecision(3) << cubePtr->sideLength << ")" << std::endl;
		}

		cubePtr->SaveASolidBox(outFile); // does the actual saving of the objects
	}

	outFile.close();
	PrintNwLnsAndLnDelimiter("-", 55);
	return 1;
}

void Utility::LoadAllObjects()
{
	if (SolidBox::cubeVec.size() != 0)
	{
		std::string strInput = "";
		char cInput = 0;
		std::regex acceptableInputExpr("^\\s*([bByYnN])\\s*$"); // looking for single character that is b, B, y, Y, n, or N
		std::cout << "Do you want to save your current data (will be overwritten) before loading a file?" << std::endl;
		std::cout << "Press 'y' to save your data before loading, 'n' to overwrite the current data with a file," << std::endl;
		std::cout << "or 'b' to go back to the main menu" << std::endl;
		strInput = GetAndValidateInput(acceptableInputExpr);
		cInput = strInput[0];

		if (tolower(cInput) == 'b')
		{
			return;
		}
		else if (tolower(cInput) == 'y')
		{
			if (!SaveAllObjects())
			{
				return;
			}

		}
		else if (tolower(cInput) == 'n')
		{
			//do nothing here since deleting and loading data will occur below
		}

		// deleting old data
		for (auto element : SolidBox::cubeVec)
		{
			element->channel.Disconnect();
		}
		SolidBox::cubeVec.clear();
	}

	SolidBox::nameIDCounter = 0; // resetting nameIDCounters in case boxes were made
	ConnectionChannel<SolidBox>::nameIDCounter = 0; // and then deleted (making SolidBox::cubeVec.size() == 0 
	SquarePlane::nameIDCounter = 0; // and the nameIDCounters != 0)
	LoadASolidBox();
	PrintNwLnsAndLnDelimiter("-", 55);
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
	std::cout << "7)  Save" << std::endl;
	std::cout << "8)  Load" << std::endl;
}

std::string Utility::CreateUniqueName(std::string strNamePrefix, int &nameIDCounter)
{
	std::string strName = "";
	strName = strNamePrefix + std::to_string(++nameIDCounter);

	auto cubeVecItr = std::find_if(SolidBox::cubeVec.begin(), SolidBox::cubeVec.end(), [&](std::shared_ptr<SolidBox> box)->bool {return box->GetShapeName() == strName; });

	try //if in set, naming collision has occurred and don't want to construct object
	{
		if (cubeVecItr != SolidBox::cubeVec.end())
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

void Utility::PrintNwLnsAndLnDelimiter(std::string strDelimiter, size_t nNumOfTimes)
{
	std::cout << "\n";

	for (size_t ii = 0; ii < nNumOfTimes; ++ii)
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
	for (auto cubePtr : SolidBox::cubeVec)
	{
		std::cout << count << ") " << cubePtr->name << std::endl;
		++count;
	}
}

bool Utility::IsOkToSave() // if no solids have been made, return false
{
	return (SolidBox::cubeVec.size() > 0);
}

void Utility::ViewFiles()
{   // checking for txt files in current directory
	WIN32_FIND_DATA file;
	HANDLE searchHandle = FindFirstFile("*.txt", &file);
	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		int count = 0;
		PrintNwLnsAndLnDelimiter("-", 55);
		std::cout << "Available files:  ";

		do
		{
			++count;
			if (count == 1)
			{
				std::cout << count << ") " << file.cFileName << std::endl;
			}
			else
			{
				std::cout << std::setw(18) << "" << count << ") " << file.cFileName << std::endl;
			}

		} while (FindNextFile(searchHandle, &file));

		FindClose(searchHandle);
	}
	else
	{
		std::cout << "No files in memory" << std::endl;
	}
}
int Utility::NumOfFilesAvail()
{
	WIN32_FIND_DATA file;
	HANDLE searchHandle = FindFirstFile("*.txt", &file);
	int count = 0;
	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			++count;

		} while (FindNextFile(searchHandle, &file));

		FindClose(searchHandle);
	}
	return count; // return # of files
}

char Utility::SaveOptions()
{
	std::string strInput = "";
	std::string fileName = "";
	char cInput = 0;
	PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Press 's' to save to an existing file (overwrite)," << std::endl;
	std::cout << "press 'n' to save to a new file," << std::endl;
	std::cout << " or press 'b' to go back to the main menu.\n" << std::endl;
	std::regex acceptableInputExpr("^\\s*([sSnNbB])\\s*$");
	strInput = GetAndValidateInput(acceptableInputExpr);
	cInput = strInput[0];
	return cInput;
}

std::string Utility::PickFile()
{
	char cInput = 0;
	std::string strInput = "";
	int numOfFiles = 0;
	int fileSelection = 0;
	WIN32_FIND_DATA file;
	HANDLE searchHandle = FindFirstFile("*.txt", &file);
	std::regex acceptableInputExpr("^\\s*([1-9]*|b|B)\\s*$");

	std::cout << "Please select a number that corresponds to the file or press 'b' to go to the main menu" << std::endl;
	strInput = GetAndValidateInput(acceptableInputExpr);
	cInput = strInput[0];

	if (tolower(cInput) == 'b')
	{
		return strInput;
	}

	numOfFiles = NumOfFilesAvail();
	ViewFiles();
	fileSelection = stoi(strInput);

	if ((fileSelection < 1) || (fileSelection > numOfFiles))
	{
		std::cout << "Invalid selection.  Please enter a valid selection" << std::endl;
		strInput = GetAndValidateInput(acceptableInputExpr);
		ViewFiles();
		numOfFiles = NumOfFilesAvail();
		fileSelection = stoi(strInput);
	}

	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		for (int ii = 1; ii < fileSelection; ++ii) // starting at 1 so you don't go to next file with 
		{                                          // FindNextFile if there's only one file or make rest
			FindNextFile(searchHandle, &file);     // of the files off by one (even if more than 1 file)
		}
	}
	else
	{
		std::cout << "A problem has occurred.  No such file in memory" << std::endl;
		return (strInput = "");
	}

	std::string fileName(file.cFileName);
	FindClose(searchHandle);
	return fileName;
}

bool Utility::FileExists(std::string fileName)
{
	bool isFound = false;
	WIN32_FIND_DATA file;
	HANDLE searchHandle = FindFirstFile(fileName.c_str(), &file);
	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		isFound = true; // matching file found
	}
	FindClose(searchHandle);
	return isFound;
}

std::string Utility::PickNewFile()
{
	std::string fileName = "";
	std::cout << "Please type the name (no extension) of the new file" << std::endl;
	std::cout << "For example, \"myFile\" would be acceptable (no quotes)" << std::endl;
	std::regex acceptableInputExpr("^\\s*([a-zA-Z0-9_]*)\\s*$");
	fileName = GetAndValidateInput(acceptableInputExpr);
	fileName += ".txt";
	if (FileExists(fileName))
	{
		std::cout << fileName << " already exists.  Please make another selection." << std::endl;
		PickNewFile();
	}
	return fileName;
}

void Utility::DeleteBox(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
{
	cubeVecItr = std::find_if(SolidBox::cubeVec.begin(), SolidBox::cubeVec.end(), [&](std::shared_ptr<SolidBox> box)->bool {return *box == **cubeVecItr; });
	if (cubeVecItr == SolidBox::cubeVec.end())
	{
		std::cout << "Cannot delete solid.  Solid not found" << std::endl;
		return;
	}
	(*cubeVecItr)->channel.Disconnect(); // setting planes in planeSet to null
	SolidBox::cubeVec.erase(cubeVecItr); // removing item from vector
}

void Utility::LoadASolidBox()
{
	int vecSize = 0;
	int nameSize = 0;
	int solidBoxNameIDCntr = 0;
	int connChannelNmIDCntr = 0;
	int sqPlnNmIDCntr = 0;
	std::string strInput = "";
	std::string strDataFromFile = "";
	std::vector<std::string> vec;
	std::ifstream inFile;
	std::vector<std::string>::iterator itr;
	std::string strLineData = "";

	ViewFiles();
	std::cout << "Please type the number corresponding to the file you with to load" << std::endl;
	std::cout << "or press 'b' to go back to the main menu" << std::endl;
	strInput = PickFile();

	if (strInput.length() == 0) // user elected to go back to the main menu
	{
		return;
	}

	inFile.open(strInput);

	while (getline(inFile, strLineData))
	{
		strDataFromFile += strLineData + ":;";
	}

	vec = TokenizeStringToVec(strDataFromFile, ';');
	std::cout << "items in vec:  " << std::endl;
	for (auto element : vec)
	{
		std::cout << element << std::endl;
	}
	itr = vec.begin();
	RetrieveInitialParams(solidBoxNameIDCntr, connChannelNmIDCntr, sqPlnNmIDCntr, vecSize, itr); // don't set NmIDCntrs permanently until
	                                                 // finished creating objects since the values will be off due to creation of objects 
	while (itr != vec.end())
	{

		if ((*itr).find("cube") != std::string::npos) // see if the string contains "cube"
			// it should if that line is for a solidbox.  If so, load it.
		{
			SolidBox::LoadSolidBox(itr, vecSize);
		}
	}

	// ensures counters are the same as when they were saved
	SolidBox::nameIDCounter = solidBoxNameIDCntr;
	ConnectionChannel<SolidBox>::nameIDCounter = connChannelNmIDCntr;
	SquarePlane::nameIDCounter = sqPlnNmIDCntr;
	inFile.close();
}

void Utility::RetrieveInitialParams(int &solidBoxNameIDCntr, int &connChannelNmIDCntr, int &sqPlnNmIDCntr, int &vecSize, std::vector<std::string>::iterator &itr)
{
	std::cout << "solidboxnameidcntr:  " << (*itr) << std::endl;
	solidBoxNameIDCntr = stoi(*itr);
	itr++; // increment iterator to go through vec for each param
	std::cout << "connchannelnmidcntr:  " << (*itr) << std::endl;
	connChannelNmIDCntr = stoi(*itr);
	itr++;
	std::cout << "sqplnnmidcntr:  " << (*itr) << std::endl;
	sqPlnNmIDCntr = stoi(*itr);
	itr++;
	std::cout << "vecsize:  " << (*itr) << std::endl;
	vecSize = stoi(*itr);
	itr++; std::cout << "in retrieve initial params. should be ':' " << (*itr) << std::endl;
	itr++; // move past ":" delimiter
}



std::vector<std::string> Utility::TokenizeStringToVec(std::string str, char delimiter)
{
	char *myDelimiter = &delimiter;
	std::vector<std::string> vec;
	char *myString = const_cast<char*>(str.c_str());
	char* nextToken = NULL;
	char *p = strtok_s(myString, myDelimiter, &nextToken);

	while (p) 
	{
		vec.push_back(p);
		p = strtok_s(NULL, myDelimiter, &nextToken);
	}
	
	return vec;
}
