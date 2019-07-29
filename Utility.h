#pragma once
#include <string>
#include <set>
#include <regex>
#include <map>
#include "SolidBox.h"
#include "SquarePlane.h"
#include <memory>

class Utility
{

public:
	static void Run();
	static std::string CreateUniqueName(std::string namePrefix, std::set<std::string> nameSet, int &nameIDCounter);

private:
	static void CreateSolidBox();
	static void WelcomeAndOptions();
	static void PrintLineDelimiter(std::string delimiter, int numOfTimes);
	static std::string RemoveSpaces(std::string input, std::regex acceptableInputExpr);
	static bool ValidateInput(std::string input, std::regex acceptableInputExpr);
	static void DebugSolidBox();
	static void ShowSolidsInMemory();
	static std::string GetAndValidateInput(std::regex acceptableInputExpr);
	static void PrintHeader(std::string header);
	static void PrintChar(char symbol, int numOfTimes);
	static void PrintDebugInfo(std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr,
		std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr);
	static void PrintCubeInfo(std::map<std::string, std::shared_ptr<SolidBox>>::iterator cubeNameAndCubeItr,
		std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr);
	static void PrintChannelInfo(std::map <std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>> > ::iterator cubeAndPlanesItr);
	static void PrintPlanesInfo(std::map<std::shared_ptr<SolidBox>, std::set<std::shared_ptr<SquarePlane>>>::iterator cubeAndPlanesItr);
	static void CopyExistingSolid();
	static void DeleteExistingSolid();
	static void MoveASolid();
	static std::map<std::string, std::shared_ptr<SolidBox>>::iterator ValAndGetCubeNmItr(std::string input, std::regex acceptableInputExpr);
	static std::string MoveInputVal(std::string input, std::regex acceptableInputExpr);
	static void SaveAllObjects();
	static void LoadAllObjects();
};

