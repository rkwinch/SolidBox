#pragma once

#include <string>
#include <fstream>
#include "Surface.h"

class ConnectionChannel;

class CurvedSurface : public Surface {

	//A curved surface is a plane-equivalent for curved surfaces (i.e. spheres).   

public:

	static const double PI;
	static int m_nNameIDCounter;

	CurvedSurface(double radius, ConnectionChannel* channel);
	CurvedSurface(const CurvedSurface &curvedSurf);
	CurvedSurface& operator=(CurvedSurface& plane);
	bool operator==(const CurvedSurface& plane) const;
	bool operator<(const CurvedSurface &plane) const;
	void CalcArea() override;
	void SetName(std::string name);
	double GetRadius() const;
	ConnectionChannel* GetConnChannel() const;
	std::shared_ptr<Surface> GetCopy() const override;
	void Save(std::ofstream &outFile) const override;
	
private:
	
	double m_dRadius = 0;
};