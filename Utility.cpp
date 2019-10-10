#include <iostream>
#include <string>
#include <regex>
#include <ctype.h>
#include <set>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include "Utility.h"
#include "Menu.h"
#include "SolidBox.h"
#include "Sphere.h"
#include "RectPrism.h"
#include "Speech.h"
#include "RectPlane.h"
#include "CurvedSurface.h"

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

		if (strInput.length() == 0 || strInput == ".")
		{
			bIsValid = false;
		}

		if (!bIsValid)
		{
			Display("Please enter valid input  ");
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
		Display("\nSay the number corresponding to your desired shape\n");
		Display("or say \"back\" to go back to the menu.\n\n");
	}
	else
	{
		Display("\nType the number corresponding to your desired shape\n");
		Display("or press 'b' to go back to the menu.\n\n");
	}

	for (auto shape : strShapeTypes)
	{
		std::ostringstream stream;
		stream << ++nCounter << ") " << shape << std::endl;
		Display(stream.str());
	}

	Display("\n");

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
		Display("\nSelection out of bounds.  Please try again.\n");
		nInput = SelectShapeType();
	}

	return nInput;
}

std::pair<std::string, int> Utility::SelectAvailableShape(int minAvailShapes)
{
	std::string strInput = "";
	std::pair<std::string, int> pair;
	int nInput = 0;
	int counter = 1;
	std::ostringstream stream;
	std::unordered_map<int, std::string> umap;
	int solidBoxVecSize = static_cast<int>(SolidBox::m_shapeVec.size());
	int sphereVecSize = static_cast<int>(Sphere::m_shapeVec.size());
	int rectPrismVecSize = static_cast<int>(RectPrism::m_shapeVec.size());
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // any number or 'b' or 'B". 0 will be checked further down
	std::vector<std::string> strShapeTypes;
	int nCounter = 0;
	Menu* menu = Menu::GetInstance();
	bool bSpeechEnabled = menu->GetIsSpeechFlag();

	if ((solidBoxVecSize < minAvailShapes) && (sphereVecSize < minAvailShapes)
		&& (rectPrismVecSize < minAvailShapes))
	{
		pair.first = "";
		pair.second = 0;
		return pair; // not enough shapes
	}

	if (bSpeechEnabled)
	{
		Display("\nSay the number corresponding to the shape you want to move\n");
		Display("or say \"back\" to go back to the menu.\n\n");
	}
	else
	{
		Display("\nType the number corresponding to the shape you want to move\n");
		Display("or press 'b' to go back to the menu.\n\n");
	}

	Utility::Display("Ex. A = B means A now contains what B contained and A is the\n");
	Utility::Display("shape you are moving TO.  B is the shape you are moving FROM.\n\n");

	if (solidBoxVecSize >= minAvailShapes)
	{
		Utility::Display(SolidBox::PrintSolids(counter));
	}

	if (sphereVecSize >= minAvailShapes)
	{
		Utility::Display(Sphere::PrintSolids(counter));
	}

	if (rectPrismVecSize >= minAvailShapes)
	{
		Utility::Display(RectPrism::PrintSolids(counter));
	}

	Display("\n");

	do {
		if (bSpeechEnabled)
		{
			nInput = Speech::RetrievePosInteger();

			if (nInput == -1)
			{
				pair.first = "";
				pair.second = 0;
				return pair; // user elected to go back to the main menu
			}
		}
		else
		{
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);

			if ((strInput == "b") || (strInput == "B"))
			{
				pair.first = "";
				pair.second = 0;
				return pair;
			}

			nInput = stoi(strInput);
		}

		if ((nInput < 1) || (nInput > counter))
		{
			Display("\nSelection out of bounds.  Please try again.\n");
		}

	} while ((nInput < 1) || (nInput > counter));

	counter = 0;

	if (solidBoxVecSize >= minAvailShapes)
	{
		for (int i = 0; i < solidBoxVecSize; ++i)
		{
			umap[++counter] = "cube";
		}
	}

	if (sphereVecSize >= minAvailShapes)
	{
		for (int i = 0; i < sphereVecSize; ++i)
		{
			umap[++counter] = "sphere";
		}
	}

	if (rectPrismVecSize >= minAvailShapes)
	{
		for (int i = 0; i < rectPrismVecSize; ++i)
		{
			umap[++counter] = "rectPrism";
		}
	}

	pair.first = umap[nInput];

	if (pair.first == "cube")
	{
		pair.second = nInput - 1;
	}
	else if (pair.first == "sphere")
	{
		if (solidBoxVecSize >= minAvailShapes)
		{
			pair.second = nInput - solidBoxVecSize - 1;
		}
		else
		{
			pair.second = nInput - 1;
		}
	}
	else // is a rectPrism
	{
		if (solidBoxVecSize >= minAvailShapes)
		{
			nInput -= solidBoxVecSize;
		}
		if (sphereVecSize >= minAvailShapes)
		{
			nInput -= sphereVecSize;
		}
		pair.second = nInput - 1;
	}
	
	return pair;
}

std::string Utility::PrintAllSolids()
{
	int count = 1;
	std::string output = "";
	std::ostringstream stream;

	if (SolidBox::m_shapeVec.size() != 0)
	{
		std::string strHeader = "SolidBox name (length of each side in mm)";
		stream << Utility::PrintHeader(strHeader);

		for (auto cube : SolidBox::m_shapeVec)
		{
			stream << cube->GetName() << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
		}

		stream << "\n" << std::endl;
	}

	count = 1;

	if (Sphere::m_shapeVec.size() != 0)
	{
		std::string strHeader = "Sphere name (length of radius in mm)";
		stream << Utility::PrintHeader(strHeader);

		for (auto sphere : Sphere::m_shapeVec)
		{
			stream << sphere->GetName() << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
		}

		stream << "\n" << std::endl;
	}

	count = 1;

	if (RectPrism::m_shapeVec.size() != 0)
	{
		std::string strHeader = "RectPrism name (length of each side in mm)";
		stream << Utility::PrintHeader(strHeader);

		for (auto rectPrism : RectPrism::m_shapeVec)
		{
			stream << rectPrism->GetName() << " (" << std::fixed << std::setprecision(3) << rectPrism->GetLength();
			stream << " x " << rectPrism->GetWidth() << " x " << rectPrism->GetHeight() << ")" << std::endl;
		}

		stream << "\n" << std::endl;
	}

	return output = stream.str();
}

std::string Utility::PrintNwLnsAndLnDelimiter(std::string strDelimiter, size_t nNumOfTimes)
{
	std::string output = "";
	std::ostringstream stream(std::ostringstream::ate);
	stream << "\n";

	for (size_t ii = 0; ii < nNumOfTimes; ++ii)
	{
		stream << strDelimiter;
	}

	stream << "\n\n";
	return output = stream.str();
}

std::string Utility::PrintChar(char cSymbol, int nNumOfTimes)
{
	std::string output = "";
	std::ostringstream stream;
	
	for (int ii = 0; ii < nNumOfTimes; ++ii)
	{
		stream << cSymbol;
	}

	return output = stream.str();
}

std::string Utility::PrintHeader(std::string strHeader)
{
	std::string output = "";
	std::ostringstream stream;
	stream << strHeader;
	stream << PrintNwLnsAndLnDelimiter("_", strHeader.length());
	return output = stream.str();
}

bool Utility::IsOkToSave() // if no shapes have been made, return false
{
	return ((SolidBox::m_shapeVec.size() > 0) || (Sphere::m_shapeVec.size() > 0));
}

std::string Utility::ViewFiles()
{   
	std::stringstream stream;
	std::string output = "";
	// checking for txt files in current directory
	WIN32_FIND_DATA file;
	HANDLE searchHandle = FindFirstFile("*.txt", &file);

	stream << std::endl;

	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		int count = 0;
		stream << "Available files:  ";

		do
		{
			++count;
			if (count == 1)
			{
				stream << count << ") " << file.cFileName << std::endl;
			}
			else
			{
				stream << std::setw(18) << "" << count << ") " << file.cFileName << std::endl;
			}

		} while (FindNextFile(searchHandle, &file));

		FindClose(searchHandle);
	}
	else
	{
		stream << PrintNwLnsAndLnDelimiter("-", 55);
		stream << "No files in memory" << std::endl;
	}

	return output = stream.str();
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

	if (isSpeech)
	{
		while ((strInput != "save") && (strInput != "new") && (strInput != "back"))
		{
			strInput = Speech::StartListeningPhrase();

			for (auto character : strInput)
			{
				character = tolower(character);
			}
		}

		if (strInput == "save") cInput = 's';
		else if (strInput == "new") cInput = 'n';
		else cInput = 'b';
	}
	else
	{
		std::regex acceptableInputExpr("^\\s*([sSnNbB])\\s*$");
		strInput = GetAndValidateInput(acceptableInputExpr);
		cInput = strInput[0];
	}

	return cInput;
}

char Utility::LoadOptions()
{
	std::string strInput = "";
	std::string fileName = "";
	char cInput = 0;
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	if (isSpeech)
	{
		while ((strInput != "yes") && (strInput != "no") && (strInput != "back"))
		{
			strInput = Speech::StartListeningPhrase();

			for (auto character : strInput)
			{
				character = tolower(character);
			}
		}

		if (strInput == "yes") cInput = 'y';
		else if (strInput == "no") cInput = 'n';
		else cInput = 'b';
	}
	else
	{
		std::regex acceptableInputExpr("^\\s*([yYnNbB])\\s*$");
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
			Display("Selection out of bounds.  Please enter a valid selection.\n");
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
		Display("A problem has occurred.  No such file in memory\n");
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
	std::vector<std::string> fileNameVec;
	std::string fileName = "";
	std::regex acceptableInputExpr("^\\s*([a-zA-Z0-9_]*)\\s*$");
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	if (isSpeech)
	{
		Display("Please say the name (no extension) of the new file.\n");
		Display("For example, \"my document\" would be acceptable (no quotes)\n");

		while (fileName == "")
		{
			fileName = Speech::StartListeningPhrase();
		}

		Speech::FileNameConversion(fileName);
	}
	else
	{
		Display("Please type the name (no extension) of the new file\n");
		Display("For example, \"myFile\" would be acceptable (no quotes)\n");
		fileName = GetAndValidateInput(acceptableInputExpr);
		fileName += ".txt";
	}

	if (FileExists(fileName))
	{
		std::ostringstream stream;
		stream << fileName << " already exists.  Please make another selection." << std::endl;
		Display(stream.str());
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
	char *element = strtok_s(myString, myDelimiter, &nextToken);

	while (element)
	{
		vec.push_back(element);
		element = strtok_s(NULL, myDelimiter, &nextToken);
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
				Display("Selection out of bounds.  Please try again or say \"back\" to go\n");
			}
			else
			{
				Display("Selection out of bounds.  Please try again or press 'b' to go\n");
			}

			Display("to the main menu.\n");
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
		return static_cast<int>(Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size() + RectPrism::m_shapeVec.size());
	}
}

std::string Utility::PrintSolidsWithConseqCntr()
{
	int count = 1;
	std::string output = "";
	std::ostringstream stream;

	if (SolidBox::m_shapeVec.size() != 0)
	{
		for (auto cube : SolidBox::m_shapeVec)
		{
			stream << Utility::PrintChar(' ', 5);
			stream << count++ << ") " << std::left << std::setw(10) << cube->GetName();
			stream << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
		}
	}

	if (Sphere::m_shapeVec.size() != 0)
	{
		for (auto sphere : Sphere::m_shapeVec)
		{
			stream << Utility::PrintChar(' ', 5);
			stream << count++ << ") " << std::left << std::setw(10) << sphere->GetName();
			stream << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
		}
	}

	if (RectPrism::m_shapeVec.size() != 0)
	{
		for (auto rectPrism : RectPrism::m_shapeVec)
		{
			stream << Utility::PrintChar(' ', 5);
			stream << count++ << ") " << std::left << std::setw(10) << rectPrism->GetName();
			stream << " (" << std::fixed << std::setprecision(3) << rectPrism->GetLength() << " x " << rectPrism->GetWidth();
			stream << " x " << rectPrism->GetHeight() << ")" << std::endl;
		}
		stream << std::endl;
	}

	return output = stream.str();
}

std::string Utility::PrintSaveOptions()
{
	std::string output = "";
	std::ostringstream stream;
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	if (isSpeech)
	{
		stream << "Say \"save\" to save an existing file (overwrite)," << std::endl;
		stream << "say \"new\" to save to a new file," << std::endl;
		stream << "or say \"back\" to go back to the main menu.\n" << std::endl;
	}
	else
	{
		stream << "Press 's' to save to an existing file (overwrite)," << std::endl;
		stream << "press 'n' to save to a new file," << std::endl;
		stream << "or press 'b' to go back to the main menu.\n" << std::endl;
	}

	return output = stream.str();
}

std::string Utility::PrintLoadOptions()
{
	std::string output = "";
	std::ostringstream stream;
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	if (isSpeech)
	{
		stream << "Say \"yes\" to save your date before loading, \"no\" to overwrite the current data with a file," << std::endl;
		stream << "or \"back\" to go back to the main menu." << std::endl;
	}
	else
	{
		stream << "Press 'y' to save your data before loading, 'n' to overwrite the current data with a file," << std::endl;
		stream << "or 'b' to go back to the main menu." << std::endl;
	}

	return output = stream.str();
}

void Utility::DeleteAllData()
{
	for (auto element : SolidBox::m_shapeVec) // deleting all SolidBoxes
	{
		auto elementPtr = dynamic_cast<SolidBox*>(element.get());
		elementPtr->GetConnChannel()->Disconnect();
	}

	SolidBox::m_shapeVec.clear();

	for (auto element : Sphere::m_shapeVec) // deleting all Spheres
	{
		auto elementPtr = dynamic_cast<Sphere*>(element.get());
		elementPtr->GetConnChannel()->Disconnect();
	}

	Sphere::m_shapeVec.clear();

	for (auto element : RectPrism::m_shapeVec) // deleting all RectPrisms
	{
		auto elementPtr = dynamic_cast<RectPrism*>(element.get());
		elementPtr->GetConnChannel()->Disconnect();
	}

	RectPrism::m_shapeVec.clear();

	// resetting counters in case shapes were made and then deleted (making shapeVec.size() == 0 and nameIDCounters != 0)
	SolidBox::m_nNameIDCounter = 0;
	Sphere::m_nNameIDCounter = 0;
	RectPrism::m_nNameIDCounter = 0;
	ConnectionChannel::m_nNameIDCounter = 0;
	RectPlane::m_nNameIDCounter = 0;
	CurvedSurface::m_nNameIDCounter = 0;
}

void Utility::Display(std::string input)
{
	std::cout << input;
}