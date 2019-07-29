#pragma once

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include "Menu.h"
#include "Surface.h"

template<class T, class M>
class Channel {
	
protected:

	T* m_shape;
	std::string m_stName = "";

public:
	
	virtual void Connect(std::set<std::shared_ptr<Surface<T,M>>> surfaceSet) = 0;
	virtual void Disconnect() = 0;
	virtual void Cleanup() = 0;
	virtual void Save(std::ofstream &outFile) = 0;
	static int m_nNameIDCounter;

	static std::string CreateUniqueName(std::string strNamePrefix, int &nNameIDCounter)
	{
		std::string strName = "";
		strName = strNamePrefix + std::to_string(++nNameIDCounter);

		auto shapeVecItr = std::find_if(T::m_shapeVec.begin(), T::m_shapeVec.end(), [&](std::shared_ptr<T> shape)->bool {return shape->GetConnName() == strName; });

		try //if in set, naming collision has occurred and don't want to construct object
		{
			if (shapeVecItr != T::m_shapeVec.end())
			{
				--nNameIDCounter;
				throw std::exception();
			}
		}
		catch (std::exception e)
		{
			std::cout << "Exception:  " << strNamePrefix << " naming collision" << std::endl;
			Menu* menu = Menu::GetInstance();
			menu->ShowSolidsInMemory();
		}
		return strName;
	}
};

template<class T, class M>
int Channel<T, M>::m_nNameIDCounter = 0;

