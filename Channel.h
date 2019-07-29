#pragma once
#include <set>
#include <string>

template<class T>
class Channel {
	
public:
	std::string m_stName = "";
	virtual void Connect(std::set<std::shared_ptr<T>> surfaceSet) = 0;
	virtual void Disconnect() = 0;
	virtual void Cleanup() = 0;
	virtual void Save(std::ofstream &outFile) = 0;
	static int m_nNameIDCounter;
	static std::string CreateUniqueName(std::string namePrefix, int &nameIDCounter)
	{
		std::string strName = "";
		strName = strNamePrefix + std::to_string(++nameIDCounter);

		auto shapeVecItr = std::find_if(T::m_shapeVec.begin(), T::m_shapeVec.end(), [&](std::shared_ptr<T> shape)->bool {return shape->GetConnName() == strName; });

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
};

template<class T>
int Channel<T>::m_nNameIDCounter = 0;

