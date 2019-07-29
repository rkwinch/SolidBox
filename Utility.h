#pragma once

#include <string>
#include <set>
#include <regex>
#include <map>
#include <memory>
#include "SolidBox.h"
#include "SquarePlane.h"
#include "ConnectionChannel.h"

class Utility
{

public:
	static void Run();
	static std::string CreateUniqueName(std::string namePrefix,  int &nameIDCounter);
	static void SaveAllObjects();
	static void LoadAllObjects();

private:
	static void CreateSolidBox();
	static void WelcomeAndOptions();
	static void PrintNwLnsAndLnDelimiter(std::string delimiter, size_t numOfTimes);
	static std::string RemoveSpaces(std::string input, std::regex acceptableInputExpr);
	static bool ValidateInput(std::string input, std::regex acceptableInputExpr);
	static void DebugSolidBox();
	static void ShowSolidsInMemory();
	static std::string GetAndValidateInput(std::regex acceptableInputExpr);
	static void PrintHeader(std::string header);
	static void PrintChar(char symbol, int numOfTimes);
	static void PrintDebugInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeAndPlanesItr);
	static void PrintCubeInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeAndPlanesItr);
	static void PrintChannelInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeAndPlanesItr);
	static void PrintPlanesInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeAndPlanesItr);
	static void CopyExistingSolid();
	static void DeleteExistingSolid();
	static void MoveASolid();
	static std::string InputInMapVal(std::string input, std::regex acceptableInputExpr);
	static void PrintSolidsInMemory();
	static void SaveASolidBox(std::shared_ptr<SolidBox> solidBoxPtr, CArchive &archive, CFile &solidBoxFile);
	static void SaveAConnectionChannel(std::shared_ptr<SolidBox> solidBoxPtr, CArchive &archive, CFile &solidBoxFile);
	static void SaveASquarePlane(std::shared_ptr<SquarePlane> plane, CArchive &archive, CFile &solidBoxFile);
	static void LoadASolidBox();
	static bool IsOkToSave();
	static int ViewFiles();
	static char SaveOptions();
	static std::string PickFileToOverWrite();
};

