#pragma once
#include <string>
#include <set>
#include <regex>
#include <map>
#include "SolidBox.h"
#include "SquarePlane.h"

//solid is moved to other solid the previous solid should be removed from the memory.
//
//Operations(d) and (e)above do not include the name of the object.Name remains a unique identifier.The name is your only way of  identifying an object.
//
//f)	Debug a solid(print information about a particular solid - includes going down to planes using the channel connection).
//
//g)	Save the current set of objects to a disk file using the Serialization protocol provided by the Microsoft Class - CObject.
//
//h)	Read an existing disk file into memory - reverse of(g).
//
//The implementation should be such that the planes are not directly accessed after construction.They should be accessed only through the solid object via the channel.

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
	static void PrintDebugInfo(std::map<std::string, SolidBox*>::iterator cubeNameAndCubeItr, std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr);
	static void PrintCubeInfo(std::map<std::string, SolidBox*>::iterator cubeNameAndCubeItr, std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr);
	static void PrintChannelInfo(std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr);
	static void PrintPlanesInfo(std::map<SolidBox*, std::set<SquarePlane*>>::iterator cubeAndPlanesItr);
	static void CopyExistingSolid();
};
