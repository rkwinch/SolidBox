#pragma once

#include <string>
#include <vector>
#include <regex>
#include <string>

class Shape;

namespace Utility {

	 static const int PRINTING_WIDTH = 30;
	 std::string PrintAllSolids();
	 int SelectShapeType();
	 std::pair<std::string, int> SelectAvailableShape(int minAvailableShapes);
	 std::vector<std::string> TokenizeStringToVec(std::string, char delimiter);
	 std::string PrintNwLnsAndLnDelimiter(std::string delimiter, size_t numOfTimes);
	 std::string RemoveSpaces(std::string input, std::regex acceptableInputExpr);
	 bool ValidateInput(std::string input, std::regex acceptableInputExpr);
	 std::string GetAndValidateInput(std::regex acceptableInputExpr);
	 std::string PrintHeader(std::string header);
	 std::string PrintChar(char symbol, int numOfTimes);
	 bool IsOkToSave();
	 std::string ViewFiles();
	 char SaveOptions();
	 char LoadOptions();
	 std::string PickFile();
	 bool FileExists(std::string fileName);
	 std::string PickNewFile();
	 int NumOfFilesAvail();
	 std::string CreateUniqueName(std::string strNamePrefix, int &nNameIDCounter);
	 int RetrieveVecInput(std::regex acceptableInputExpr, int shapeVecSize);
	 std::string GetShapeType(Shape* shape);
	 int AvailableSolids();
	 std::string PrintSolidsWithConseqCntr();
	 std::string PrintSaveOptions();
	 std::string PrintLoadOptions();
	 void DeleteAllData();
	 void Display(std::string input);
}