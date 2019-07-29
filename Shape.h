#pragma once

#include <vector>
#include "RectPlane.h"
#include <string>

//abstract.  Don't make instances of Shape.
template<class T, class  M>
class Shape {

protected:
	static int m_nNameIDCounter; // used for naming unique cubes; initialized at the bottom of this header
	std::string m_stName = "";
	bool m_bHasConnection = false; // flag for checking if the shape has a connection
	ConnectionChannel<T, M> m_channel;

	virtual T& operator=(T &cube) = 0;
	static void Load(std::vector<std::string>::iterator &itr, const int &vecSize) = 0;
	void Save(std::ofstream &outFile) = 0;

	ConnectionChannel<T>* GetConnChannel()
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
};

template<class T>
int Shape<T>::m_nNameIDCounter = 0; // initializing static member of template class here

