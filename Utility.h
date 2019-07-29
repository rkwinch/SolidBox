#pragma once
#include <string>
#include <set>

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
	static bool ValidateMenuInput(std::string input);
	static void Utility::DebugSolidBox();
	static std::string Utility::CreateUniqueName(std::string namePrefix, std::set<std::string> nameSet, int &nameIDCounter);
	static void Utility::ShowSolidsInMemory();
	static void Utility::PrintLineDelimiter(std::string delimiter, int numOfTimes);

private:
	static bool ValidateSideLengthInput(std::string input);
	static void CreateSolidBox();
	static void WelcomeAndOptions();
	static char MenuInputToChar(std::string input);

};
