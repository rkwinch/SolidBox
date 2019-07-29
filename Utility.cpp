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
#include "RectPlane.h"
#include "SolidBox.h"
#include "Utility.h"
#include "Sphere.h"
#include "Menu.h"

std::string Utility<T>::CreateUniqueName(std::string strNamePrefix, int &nameIDCounter)
{
	std::string strName = "";
	strName = strNamePrefix + std::to_string(++nameIDCounter);

	auto cubeVecItr = std::find_if(SolidBox::m_shapeVec.begin(), SolidBox::m_shapeVec.end(), [&](std::shared_ptr<SolidBox> box)->bool {return box->GetShapeName() == strName; });
	auto sphereVecItr = std::find_if(Sphere::m_shapeVec.begin(), Sphere::m_shapeVec.end(), [&](std::shared_ptr<Sphere> sphere)->bool {return sphere->GetShapeName() == strName; });

	try //if in set, naming collision has occurred and don't want to construct object
	{
		if ((cubeVecItr != SolidBox::m_shapeVec.end()) || (sphereVecItr != Sphere::m_shapeVec.end()))
		{
			throw std::exception();
		}
	}
	catch (std::exception e)
	{
		std::cout << "Exception:  " << strNamePrefix << " naming collision" << std::endl;
		Menu* menu = menu->GetInstance();
		menu->ShowSolidsInMemory();
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

std::string Utility::InputInVecVal(std::string strInput, std::regex acceptableInputExpr, std::set<std::shared_ptr<T> shapeVec)
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

bool Utility::IsOkToSave() // if no shapes have been made, return false
{
	return ((SolidBox::shapeVec.size() > 0) || (Sphere::shapeVec.size() > 0));
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
