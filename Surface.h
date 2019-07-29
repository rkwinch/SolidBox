#pragma once

#include <string>
#include "Menu.h"

template<class T, class M>
class ConnectionChannel;

template<class T, class M>
class Surface {

protected:

	std::string m_stName = "";
	ConnectionChannel<T, M>* m_channel;
	int m_nNumOfEdges = 0;

public:
	
	double m_dArea = 0;
	virtual double CalcArea() = 0;
	
	virtual std::shared_ptr<M> GetCopy() = 0;

	static std::string CreateUniqueName(std::string strNamePrefix, int &nNameIDCounter)
	{
		std::string strName = "";
		strName = strNamePrefix + std::to_string(++nNameIDCounter);

		auto shapeVecItr = std::find_if(T::m_shapeVec.begin(), T::m_shapeVec.end(), [&](std::shared_ptr<T> shape)->bool
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
