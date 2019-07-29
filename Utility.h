#pragma once

#include <string>
#include <set>
#include <regex>
#include <memory>
#include "SolidBox.h"
#include "RectPlane.h"
#include "ConnectionChannel.h"

template<class T> // T for shape type
class Utility {

public:
	
	static std::string CreateUniqueName(std::string namePrefix, int &nameIDCounter)
	{
		std::string strName = "";
		strName = strNamePrefix + std::to_string(++nameIDCounter);

		auto shapeVecItr = std::find_if(T::m_shapeVec.begin(), T::m_shapeVec.end(), [&](std::shared_ptr<T> shape)->bool {return shape->GetShapeName() == strName; });
		if (shapeVecItr == T::m_shapeVec.end())
		{
			shapeVecItr = std::find_if(T::m_shapeVec.begin(), T::m_shapeVec.end(), [&](std::shared_ptr<T> shape)->bool {return shape->GetConnName() == strName; });
			if (shapeVecItr == T::m_shapeVec.end())
			{
				shapeVecItr = std::find_if(T::m_shapeVec.begin(), T::m_shapeVec.end(), [&](std::shared_ptr<T> shape)->bool 
				{
					for (auto element : shape->m_channel.GetSurfaceSet)
					{
						if (element->GetSurfaceName() == strName)
						{
							return true;
						}
					}
					return false;
				});		
			}
		}

		try //if in set, naming collision has occurred and don't want to construct object
		{
			if (shapeVecItr != T::m_shapeVec.end())
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

	static std::vector<std::string> TokenizeStringToVec(std::string, char delimiter);
	static void PrintNwLnsAndLnDelimiter(std::string delimiter, size_t numOfTimes);
	static std::string RemoveSpaces(std::string input, std::regex acceptableInputExpr);
	static bool ValidateInput(std::string input, std::regex acceptableInputExpr);
	static std::string GetAndValidateInput(std::regex acceptableInputExpr);
	static void PrintHeader(std::string header);
	static void PrintChar(char symbol, int numOfTimes);
	static std::string InputInVecVal(std::string input, std::regex acceptableInputExpr);
	static bool IsOkToSave();
	static void ViewFiles();
	static char SaveOptions();
	static std::string PickFile();
	static bool FileExists(std::string fileName);
	static std::string PickNewFile();
	static int NumOfFilesAvail();
};

