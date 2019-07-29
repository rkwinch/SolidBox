#pragma once

#include <set>
#include <vector>
#include <string>
#include "Utility.h"
#include "ConnectionChannel.h"

//abstract.  Don't make instances of Shape.
template<class T, class  M>
class Shape {

protected:

	//ConnectionChannel<T, M> m_channel; fix this later
	std::string m_stName = "";
	bool m_bHasConnection = false; // flag for checking if the shape has a connection

public:

	static int m_nNameIDCounter; // used for naming unique cubes; initialized at the bottom of this header
	static const int m_nNumOfSurfaces = 0;
	static std::vector<std::shared_ptr<Shape<T, M>>> m_shapeVec;
	virtual T& operator=(T &cube) = 0;
	virtual int GetSurfaceCount() = 0;
	virtual std::set<std::shared_ptr<M>> GetSurfacesCopy() = 0;
	virtual void Save(std::ofstream &outFile) = 0; 
	virtual void Delete() = 0;

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

	std::string GetShapeName()
	{
		return m_stName;
	}

	static std::string CreateUniqueName(std::string strNamePrefix, int &nNameIDCounter)
	{
		std::string strName = "";
		strName = strNamePrefix + std::to_string(++nNameIDCounter);

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
			Utility::PrintAllSolids();
		}
		return strName;
	}

	static std::string InputInVecVal(std::string strInput, std::regex acceptableInputExpr, std::vector<std::shared_ptr<Shape<T,M>>> shapeVec)
	{
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);

		if ((strInput == "b") | (strInput == "B")) // user elected to go back to main menu
		{
			return "b";
		}

		// check if shape exists
		while ((size_t(stoi(strInput)) > T::m_shapeVec.size()) || (stoi(strInput) < 1))
		{
			std::cout << "Selection out of bounds.  Please try again or press 'b' to go" << std::endl;
			std::cout << "to the main menu." << std::endl;
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);

			if ((strInput == "b") | (strInput == "B")) // user elected to go back to main menu
			{
				return "b";
			}

		}
		return strInput;
	}

	std::vector<std::shared_ptr<T>> GetShapeVec()
	{
		return T::m_shapeVec;
	}

	bool GetHasConnection()
	{
		return m_bHasConnection;
	}
	
	//virtual static void Load(std::vector<std::string>::iterator &itr, const int &vecSize) = 0; 		
};

template<class T, class M>
int Shape<T, M>::m_nNameIDCounter = 0; // initializing static member of template class here

