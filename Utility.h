#pragma once

#include <string>
#include <set>
#include <regex>

class Utility {

public:
	
	static void PrintAllSolids();
	static std::string SelectShapeType();
	static std::vector<std::string> TokenizeStringToVec(std::string, char delimiter);
	static void PrintNwLnsAndLnDelimiter(std::string delimiter, size_t numOfTimes);
	static std::string RemoveSpaces(std::string input, std::regex acceptableInputExpr);
	static bool ValidateInput(std::string input, std::regex acceptableInputExpr);
	static std::string GetAndValidateInput(std::regex acceptableInputExpr);
	static void PrintHeader(std::string header);
	static void PrintChar(char symbol, int numOfTimes);
	static bool IsOkToSave();
	static void ViewFiles();
	static char SaveOptions();
	static std::string PickFile();
	static bool FileExists(std::string fileName);
	static std::string PickNewFile();
	static int NumOfFilesAvail();
};

