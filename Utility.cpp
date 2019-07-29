#include <iostream>
#include <string>
#include <regex>
#include <ctype.h>
#include <set>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <fstream>
#include <Windows.h>
#include "Utility.h"
#include "Menu.h"
#include "SolidBox.h"
#include "Sphere.h"

class ConnectionChannel;

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

//only for one capture group
std::string Utility::RemoveSpaces(std::string strInput, std::regex acceptableInputExpr)
{
	strInput = std::regex_replace(strInput, acceptableInputExpr, "$1");
	return strInput;
}

//only for one capture group
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

std::string Utility::SelectShapeType()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // any number or 'b' or 'B". 0 will be checked further down
	std::vector<std::string> strShapeTypes{ "Cube", "Sphere" };
	int nCounter = 0;

	for (auto shape : strShapeTypes)
	{
		std::cout << ++nCounter << ") " << shape << std::endl;
	}

	std::cout << "Type the number corresponding to your desired shape" << std::endl;
	std::cout << "or press 'b' to go back to the menu.\n" << std::endl;

	strInput = Utility::GetAndValidateInput(acceptableInputExpr);

	if ((strInput == "b") || (strInput == "B"))
	{
		return strInput;
	}

	nInput = stoi(strInput);

	if ((nInput < 1) || (static_cast<size_t>(nInput) > strShapeTypes.size()))
	{
		std::cout << "Selection out of bounds.  Please try again." << std::endl;
		SelectShapeType();
	}

	return strInput;
}

void Utility::PrintAllSolids()
{
	int count = 1;

	if (SolidBox::m_shapeVec.size() != 0)
	{
		std::string strHeader = "SolidBox name (length of each side in mm)";
		Utility::PrintHeader(strHeader);

		for (auto cube : SolidBox::m_shapeVec)
		{
			std::cout << cube->GetName() << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
		}
		std::cout << std::endl;
	}

	count = 1;

	if (Sphere::m_shapeVec.size() != 0)
	{
		std::string strHeader = "Sphere name (length of each side in mm)";
		Utility::PrintHeader(strHeader);

		for (auto sphere : Sphere::m_shapeVec)
		{
			std::cout << sphere->GetName() << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
		}
		std::cout << std::endl;
	}
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

bool Utility::IsOkToSave() // if no shapes have been made, return false
{
	return ((SolidBox::m_shapeVec.size() > 0) || (Sphere::m_shapeVec.size() > 0));
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

std::string Utility::CreateUniqueName(std::string strNamePrefix, int &nNameIDCounter)
{
	std::string strName = "";
	strName = strNamePrefix + std::to_string(++nNameIDCounter);
	return strName;
}

// getting user input and validating input based on if the shape exists...helper fxn
std::string Utility::InputInVecVal(std::string strInput, std::regex acceptableInputExpr, int shapeVecSize)
{
	strInput = Utility::GetAndValidateInput(acceptableInputExpr);

	if ((strInput == "b") | (strInput == "B")) // user elected to go back to main menu
	{
		return "b";
	}

	// check if shape exists
	while ((size_t(stoi(strInput)) > static_cast<size_t>(shapeVecSize)) || (stoi(strInput) < 1))
	{
		std::cout << "Selection out of bounds.  Please try again or press 'b' to go" << std::endl;
		std::cout << "to the main menu." << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);

		if ((strInput == "b") | (strInput == "B")) // user elected to go back to main menu
		{
			return "b";
		}

	}

	return strInput;
}

std::string Utility::GetShapeType(Shape* shape)
{
	int counter = 0;
	std::string name = shape->GetName();
	std::string namePrefix = "";
	while (!isdigit(name[counter]))
	{
		namePrefix += name[counter];
		++counter;
	}
	return namePrefix;
}