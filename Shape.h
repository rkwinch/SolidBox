#pragma once

#include <vector>
#include "RectPlane.h"
#include <string>

//abstract.  Don't make instances of Shape.
template<class T, class  M>
class Shape {
public:
	static const int m_nNumOfSurfaces = 0;
	virtual int GetSurfaceCount() = 0;
	virtual std::set<std::shared_ptr<M>> GetSurfacesCopy() = 0;
	virtual void Save(std::ofstream &outFile) = 0; 

	static std::string CreateUniqueName(std::string namePrefix, int &nameIDCounter)
	{
		std::string strName = "";
		strName = strNamePrefix + std::to_string(++nameIDCounter);

		auto shapeVecItr = std::find_if(T::m_shapeVec.begin(), T::m_shapeVec.end(), [&](std::shared_ptr<T> shape)->bool {return shape->GetShapeName() == strName; });

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

protected:
	static int m_nNameIDCounter; // used for naming unique cubes; initialized at the bottom of this header
	std::string m_stName = "";
	bool m_bHasConnection = false; // flag for checking if the shape has a connection
	ConnectionChannel<T, M> m_channel;

	virtual T& operator=(T &cube) = 0;
	//virtual static void Load(std::vector<std::string>::iterator &itr, const int &vecSize) = 0;
	virtual void Save(std::ofstream &outFile) = 0; 
	virtual std::vector<std::shared_ptr<T>> GetShapeVec() = 0;

	ConnectionChannel<T, M>* GetConnChannel()
	{
		return &m_channel;
	}

	bool operator<(const T &shape) const
	{
		return (this->m_stName < shape.m_stName);
	}

	bool operator==(const T &shape) const
	{
		return (this->m_stName == shape.m_stName);
	}

	void SetName(std::string m_stName)
	{
		this->m_stName = m_stName;
	}

	virtual std::string GetShapeName()
	{
		return m_stName;
	}

	bool GetHasConnection()
	{
		return m_bHasConnection;
	}	

	void Delete()
	{
		std::vector<std::shared_ptr<T>>::iterator surfaceVecItr = surfaceVec.begin();
		// [&] is take by reference, arg type is shared ptr of surface type (solidbox or sphere at this point), return type is bool, 
		// predicate is check if the surfaces are equivalent (same name by == operator)
		surfaceVecItr = std::find_if(surfaceVec.begin(), surfaceVec.end(), [&](std::shared_ptr<T> surface)->bool {return *surface == *this; });
		if (surfaceVecItr == surfaceVec.end())
		{
			std::cout << "Cannot delete solid.  Solid not found" << std::endl;
			return;
		}
		m_channel.Disconnect(); // setting surfaces in surfaceSet to null
		T::surfaceVec.erase(surfaceVecItr); // removing item from vector
	}
};

template<class T, class M>
int Shape<T, M>::m_nNameIDCounter = 0; // initializing static member of template class here

