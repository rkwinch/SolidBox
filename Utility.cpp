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
#include "RectPrism.h"
#include "Speech.h"

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

int Utility::SelectShapeType()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // any number or 'b' or 'B". 0 will be checked further down
	std::vector<std::string> strShapeTypes{ "Cube", "Sphere", "RectPrism" };
	int nCounter = 0;
	Menu* menu = Menu::GetInstance();
	bool bSpeechEnabled = menu->GetIsSpeechFlag();

	if (bSpeechEnabled)
	{
		std::cout << "\nSay the number corresponding to your desired shape" << std::endl;
		std::cout << "or say \"back\" to go back to the menu.\n" << std::endl;
	}
	else
	{
		std::cout << "\nType the number corresponding to your desired shape" << std::endl;
		std::cout << "or press 'b' to go back to the menu.\n" << std::endl;
	}

	for (auto shape : strShapeTypes)
	{
		std::cout << ++nCounter << ") " << shape << std::endl;
	}

	std::cout << std::endl;

	if (bSpeechEnabled)
	{
		nInput = Speech::RetrievePosInteger();
		
		if (nInput == -1) // user elected to go back to the main menu
		{
			return -1;
		}
	}
	else
	{
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		
		if ((strInput == "b") || (strInput == "B"))
		{
			return -1;
		}

		nInput = stoi(strInput);
	}
	
	if ((nInput < 1) || (static_cast<size_t>(nInput) > strShapeTypes.size()))
	{
		std::cout << "\nSelection out of bounds.  Please try again." << std::endl;
		nInput = SelectShapeType();
	}
	
	return nInput;
}

std::string Utility::SelectAvailableShapeType(int minAvailShapes)
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // any number or 'b' or 'B". 0 will be checked further down
	std::vector<std::string> strShapeTypes;
	int nCounter = 0;
	Menu* menu = Menu::GetInstance();
	bool bSpeechEnabled = menu->GetIsSpeechFlag();

	if ((SolidBox::m_shapeVec.size() < minAvailShapes) && (Sphere::m_shapeVec.size() < minAvailShapes)
		&& (RectPrism::m_shapeVec.size() < minAvailShapes))
	{
		return ""; // not enough shapes
	}

	if (bSpeechEnabled)
	{
		std::cout << "\nSay the number corresponding to your desired shape" << std::endl;
		std::cout << "or say \"back\" to go back to the menu.\n" << std::endl;
	}
	else
	{
		std::cout << "\nType the number corresponding to your desired shape" << std::endl;
		std::cout << "or press 'b' to go back to the menu.\n" << std::endl;
	}

	if (SolidBox::m_shapeVec.size() >= minAvailShapes) strShapeTypes.push_back("cube");

	if (Sphere::m_shapeVec.size() >= minAvailShapes) strShapeTypes.push_back("sphere");

	if (RectPrism::m_shapeVec.size() >= minAvailShapes) strShapeTypes.push_back("rectPrism");

	for (auto shape : strShapeTypes)
	{
		std::cout << ++nCounter << ") " << shape << std::endl;
	}

	std::cout << std::endl;

	do {
		if (bSpeechEnabled)
		{
			nInput = Speech::RetrievePosInteger();

			if (nInput == -1) return ""; // user elected to go back to the main menu
		}
		else
		{
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);

			if ((strInput == "b") || (strInput == "B")) return 0;

			nInput = stoi(strInput);
		}

		if (nInput < 1 || static_cast<size_t>(nInput) > strShapeTypes.size())
		{
			std::cout << "\nSelection out of bounds.  Please try again." << std::endl;
		}

	}while ((nInput < 1) || (static_cast<size_t>(nInput) > strShapeTypes.size()));
	
	return strShapeTypes[nInput - 1];
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

		std::cout << "\n" << std::endl;
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

		std::cout << "\n" << std::endl;
	}

	count = 1;

	if (RectPrism::m_shapeVec.size() != 0)
	{
		std::string strHeader = "RectPrism name (length of each side in mm)";
		Utility::PrintHeader(strHeader);

		for (auto rectPrism : RectPrism::m_shapeVec)
		{
			std::cout << rectPrism->GetName() << " (" << std::fixed << std::setprecision(3) << rectPrism->GetLength();
			std::cout << " x " << rectPrism->GetWidth() << " x " << rectPrism->GetHeight() << ")" << std::endl;
		}

		std::cout << "\n" << std::endl;
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

	std::cout << std::endl;

	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		int count = 0;
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
		PrintNwLnsAndLnDelimiter("-", 55);
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
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	PrintNwLnsAndLnDelimiter("-", 55);

	if (isSpeech)
	{
		std::cout << "Say \"save\" to save an existing file (overwrite)," << std::endl;
		std::cout << "say \"new\" to save to a new file," << std::endl;
		std::cout << "or say \"back\" to go back to the main menu.\n" << std::endl;

		while ((strInput != "save") || (strInput != "new") || (strInput != "back"))
		{
			strInput = Speech::StartListening();

			for (auto character : strInput)
			{
				character = tolower(character);
			}
		}

		if (strInput == "save") return 's';
		else if (strInput == "new") return 'n';
		else return 'b';
	}
	else
	{
		std::cout << "Press 's' to save to an existing file (overwrite)," << std::endl;
		std::cout << "press 'n' to save to a new file," << std::endl;
		std::cout << "or press 'b' to go back to the main menu.\n" << std::endl;
		std::regex acceptableInputExpr("^\\s*([sSnNbB])\\s*$");
		strInput = GetAndValidateInput(acceptableInputExpr);
		cInput = strInput[0];
	}
	
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
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	do
	{
		if (isSpeech)
		{
			fileSelection = Speech::RetrievePosInteger();
			
			if (fileSelection == -1) return "b";
		}
		else
		{
			strInput = GetAndValidateInput(acceptableInputExpr);
			cInput = strInput[0];

			if (tolower(cInput) == 'b') return "b";

			fileSelection = stoi(strInput);
		}

		numOfFiles = NumOfFilesAvail();

		if ((fileSelection < 1) || (fileSelection > numOfFiles))
		{
			std::cout << "Selection out of bounds.  Please enter a valid selection." << std::endl;
		}

	} while ((fileSelection < 1) || (fileSelection > numOfFiles));

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
	std::regex acceptableInputExpr("^\\s*([a-zA-Z0-9_]*)\\s*$");

	std::cout << "Please type the name (no extension) of the new file" << std::endl;
	std::cout << "For example, \"myFile\" would be acceptable (no quotes)" << std::endl;
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

std::string Utility::FilterNonChars(std::string input)
{
	std::string retString = input;

	for (size_t i = 0; i < retString.length(); ++i)
	{
		if (!isalpha(retString[i]))
		{
			retString[i] = ' ';
		}
	}

	return retString;
}

std::string Utility::CreateUniqueName(std::string strNamePrefix, int &nNameIDCounter)
{
	std::string strName = "";
	strName = strNamePrefix + std::to_string(++nNameIDCounter);
	return strName;
}

// getting user input and validating input based on if the shape exists...helper fxn
int Utility::RetrieveVecInput(std::regex acceptableInputExpr, int shapeVecSize)
{
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();
	std::string strInput = "";
	int nInput = 0;

	do
	{
		if (isSpeech)
		{
			nInput = Speech::RetrievePosInteger();
			
			if (nInput == -1) return -1; // user elected to go back to main menu
		}
		else
		{
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);
			
			if ((strInput == "b") | (strInput == "B")) return -1; // user elected to go back to main menu

			nInput = stoi(strInput);
		}

		if ((nInput > shapeVecSize) || (nInput < 1))
		{
			if (isSpeech)
			{
				std::cout << "Selection out of bounds.  Please try again or say \"back\" to go" << std::endl;
			}
			else
			{
				std::cout << "Selection out of bounds.  Please try again or press 'b' to go" << std::endl;
			}

			std::cout << "to the main menu." << std::endl;
		}
	} while ((nInput > shapeVecSize) || (nInput < 1)); // check if shape exists

	return nInput;
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

int Utility::AvailableSolids()
{
	if ((Sphere::m_shapeVec.size() == 0) && (SolidBox::m_shapeVec.size() == 0) && (RectPrism::m_shapeVec.size() == 0))
	{
		return 0;
	}
	else
	{
		return (Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size() + RectPrism::m_shapeVec.size());
	}
}

void Utility::PrintSolidsWithConseqCntr()
{
	int count = 1;

	if (SolidBox::m_shapeVec.size() != 0)
	{
		for (auto cube : SolidBox::m_shapeVec)
		{
			Utility::PrintChar(' ', 5);
			std::cout << count++ << ") " << std::left << std::setw(10) << cube->GetName();
			std::cout << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
		}
	}

	if (Sphere::m_shapeVec.size() != 0)
	{
		for (auto sphere : Sphere::m_shapeVec)
		{
			Utility::PrintChar(' ', 5);
			std::cout << count++ << ") " << std::left << std::setw(10) << sphere->GetName();
			std::cout << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
		}
	}

	if (RectPrism::m_shapeVec.size() != 0)
	{
		for (auto rectPrism : RectPrism::m_shapeVec)
		{
			Utility::PrintChar(' ', 5);
			std::cout << count++ << ") " << std::left << std::setw(10) << rectPrism->GetName();
			std::cout << " (" << std::fixed << std::setprecision(3) << rectPrism->GetLength() << " x " << rectPrism->GetWidth();
			std::cout << " x " << rectPrism->GetHeight() << ")" << std::endl;
		}
		std::cout << std::endl;
	}
}