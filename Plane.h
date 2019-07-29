#pragma once

#include <string>

template<class T, class M>
class ConnectionChannel;

template<class T>
class Plane {
protected:
	std::string m_stName = "";
	ConnectionChannel<T> m_channel;
	static int m_nNameIDCounter;
	double d_Area = 0;
	double CalcArea() = 0;
	int m_nNumOfEdges = 0;
};

template<class T>
int Plane<T>::m_nNameIDCounter = 0; // initializing static member of template class here
