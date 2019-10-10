#pragma once

#include <string>
#include <fstream>
#include "Surface.h"

class ConnectionChannel;

//A rectangular (rect) plane is a plane that can be square or rectangular.  
//Each plane is a 2D rectangle that has 4 edges that are
//90' with a height and a length.

class RectPlane : public Surface {

private:

	double m_dHeight = 0;
	double m_dLength = 0;

public:

	static int m_nNameIDCounter; 

	RectPlane(double length, double height, ConnectionChannel* channel);
	RectPlane(const RectPlane &rectPlane);
	RectPlane& operator=(RectPlane& plane);
	bool operator==(const RectPlane& plane) const;
	bool operator<(const RectPlane &plane) const;
	void CalcArea() override; 
	void SetName(std::string name); 
	double GetLength() const;
	double GetHeight() const;
	std::shared_ptr<Surface> GetCopy() const override;
	void Save(std::ofstream &outFile) const override;
};