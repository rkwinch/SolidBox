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
	CurvedSurface& operator=(CurvedSurface& plane);
	bool operator==(const CurvedSurface& plane) const;
	bool operator<(const CurvedSurface &plane) const;
	void SetName(std::string name);
	void SetRadius(double radius);
	double GetRadius();
	ConnectionChannel* GetConnChannel();
	std::shared_ptr<Surface> GetCopy() override;
	void Save(std::ofstream &outFile) override;
	
private:

	double m_dRadius;

	double CalcArea() const override;
};