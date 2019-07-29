#pragma once

#include <string>
#include <vector>
#include <regex>

class Shape;

namespace Utility {

	 void PrintAllSolids();
	 std::string SelectShapeType();
	 std::vector<std::string> TokenizeStringToVec(std::string, char delimiter);
	 std::string FilterNonChars(std::string input);
	 void PrintNwLnsAndLnDelimiter(std::string delimiter, size_t numOfTimes);
	 std::string RemoveSpaces(std::string input, std::regex acceptableInputExpr);
	 bool ValidateInput(std::string input, std::regex acceptableInputExpr);
	 std::string GetAndValidateInput(std::regex acceptableInputExpr);
	 void PrintHeader(std::string header);
	 void PrintChar(char symbol, int numOfTimes);
	 bool IsOkToSave();
	 void ViewFiles();
	 char SaveOptions();
	 std::string PickFile();
	 bool FileExists(std::string fileName);
	 std::string PickNewFile();
	 int NumOfFilesAvail();
	 std::string CreateUniqueName(std::string strNamePrefix, int &nNameIDCounter);
	 std::string InputInVecVal(std::string strInput, std::regex acceptableInputExpr, int shapeVecSize);
	 std::string GetShapeType(Shape* shape);
	 int AvailableSolids();
	 void PrintSolidsWithConseqCntr();
	
}