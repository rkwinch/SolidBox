#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "Utility.h"
#include "RectPrism.h"
#include "RectPlane.h"
#include "ConnectionChannel.h"

const int RectPrism::m_nSurfaces = 6;
int RectPrism::m_nNameIDCounter = 0;
std::vector<std::shared_ptr<RectPrism>> RectPrism::m_shapeVec;

RectPrism::RectPrism(double length, double width, double height)
{
	//giving the cube a unique name where it is guaranteed to be unique due to the nameIDCounter.
	//will verify by putting the name into a set and check if it properly inserts.
	m_stName = Utility::CreateUniqueName("rectPrism", m_nNameIDCounter);
	m_nNumOfSurfaces = 6;
	std::set<std::shared_ptr<Surface>> rectPlaneSet;

	//making all planes (opp planes are the same.  6 planes total, so potential 3 diff unique planes)
	for (int i = 0; i < 2; ++i)
	{
		std::shared_ptr<RectPlane> plane = std::make_shared<RectPlane>(length, height, &m_channel);
		std::shared_ptr<RectPlane> plane2 = std::make_shared<RectPlane>(width, height, &m_channel);
		std::shared_ptr<RectPlane> plane3 = std::make_shared<RectPlane>(length, width, &m_channel);
		auto insert = rectPlaneSet.insert(plane);
		auto insert2 = rectPlaneSet.insert(plane2);
		auto insert3 = rectPlaneSet.insert(plane3);

		if ((insert.second == false) || (insert2.second == false) || (insert3.second == false))
		{
			throw std::exception("Cannot make RectPrism.  Duplicate planes detected");
		}
	}

	m_dHeight = height;
	m_dLength = length;
	m_dWidth = width;
	CalcSA();
	CalcVol();
	m_channel.Connect(rectPlaneSet);
	//if all planes inserted correctly, then a proper ChannelConnection has been made
	m_bHasConnection = true;
}

RectPrism::~RectPrism()
{
	// don't need to do anything here since memory is handled via smart pointers
}

RectPrism::RectPrism(RectPrism& rectPrism)
{
	// don't change name
	m_dLength = rectPrism.m_dLength;
	m_dHeight = rectPrism.m_dHeight;
	m_dWidth = rectPrism.m_dWidth;
	m_channel = rectPrism.m_channel;
	m_bHasConnection = rectPrism.m_bHasConnection; 
}

RectPrism& RectPrism::operator=(RectPrism &rectPrism)
{
	m_dLength = rectPrism.m_dLength;
	m_dHeight = rectPrism.m_dHeight;
	m_dWidth = rectPrism.m_dWidth;
	m_channel = rectPrism.m_channel;
	m_bHasConnection = rectPrism.m_bHasConnection;
	rectPrism.Delete(); //**deleting items on right side of = operator**
	return *this;
}

void RectPrism::Delete()
{
	std::vector<std::shared_ptr<RectPrism>>::iterator shapeVecItr = m_shapeVec.begin();
	shapeVecItr = std::find_if(m_shapeVec.begin(), m_shapeVec.end(), [&](std::shared_ptr<RectPrism> shape)->bool {return *shape == *this; });

	if (shapeVecItr == m_shapeVec.end())
	{
		std::cout << "Cannot delete Shape.  Shape not found." << std::endl;
		return;
	}

	m_channel.Disconnect(); // setting surfaces in surfaceSet to null
	m_shapeVec.erase(shapeVecItr); // removing item from vector
}

void RectPrism::CalcVol()
{   // vol = length * width * height
	m_dVolume = m_dLength * m_dWidth * m_dHeight;
}

void RectPrism::CalcSA()
{    // surface area = 2wl + 2 hl + 2hw
	m_dSurfaceArea = 2.0 * (m_dWidth * m_dLength + m_dHeight * m_dLength + m_dHeight * m_dWidth);
}

double RectPrism::GetLength()
{
	return m_dLength;
}

double RectPrism::GetHeight()
{
	return m_dHeight;
}

double RectPrism::GetWidth()
{
	return m_dWidth;
}

std::set<std::shared_ptr<Surface>> RectPrism::GetSurfacesCopy()
{
	std::set<std::shared_ptr<Surface>> surfaceSet;

	for (auto surface : m_channel.GetSurfaceSet())
	{
		auto element = dynamic_cast<RectPlane*>(surface.get());
		std::shared_ptr<RectPlane> copy = std::make_shared<RectPlane>(element->GetHeight(), element->GetLength(), &m_channel);
		surfaceSet.insert(copy); // don't need to check if unique name or not since was already in a set
	}
	return surfaceSet;
}

std::vector<std::shared_ptr<RectPrism>> RectPrism::GetShapeVec()
{
	return m_shapeVec;
}

void RectPrism::Save(std::ofstream &outFile) \
{
	outFile << m_stName << ";" << m_dLength << ";";
	outFile << m_dWidth << ";" << m_dHeight << ";" << m_bHasConnection << ";";
	m_channel.Save(outFile); // takes care of channel member and its associated surfaces
	outFile << "\n";
}

void RectPrism::Create()
{
	std::string strInput3 = "";
	std::string strInput2 = "";
	std::string strInput = "";
	double dLength = 0.0;
	double dHeight = 0.0;
	double dWidth = 0.0;
	std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
																	// with 0-1 decimals followed by a number (if present) while allowing spaces
	std::cout << "What would you like the length to be? (in mm)" << std::endl;
	std::cout << "ex: 3.3" << std::endl;
	strInput = Utility::GetAndValidateInput(acceptableInputExpr);
	dLength = std::stod(strInput); // converting string input into a double

	while (dLength == 0.0) // don't want to make a box with sideLength of 0
	{
		std::cout << "Please input a value that is not 0" << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		dLength = std::stod(strInput);
	}

	std::cout << "What would you like the width to be? (in mm)" << std::endl;
	strInput3 = Utility::GetAndValidateInput(acceptableInputExpr);
	dWidth = std::stod(strInput3);

	while (dWidth == 0.0)
	{
		std::cout << "Please input a value that is not 0" << std::endl;
		strInput3 = Utility::GetAndValidateInput(acceptableInputExpr);
		dWidth = std::stod(strInput3);
	}

	std::cout << "What would you like the height to be? (in mm)" << std::endl;
	strInput2 = Utility::GetAndValidateInput(acceptableInputExpr);
	dHeight = std::stod(strInput2);

	while (dHeight == 0.0)
	{
		std::cout << "Please input a value that is not 0" << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		dHeight = std::stod(strInput2);
	}

	std::shared_ptr<RectPrism> rectPrism = std::make_shared<RectPrism>(dLength, dWidth, dHeight);
	m_shapeVec.push_back(rectPrism);
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void RectPrism::PrintSolids()
{
	int count = 1;

	for (auto rectPrism : m_shapeVec)
	{
		std::cout << count << ") " << rectPrism->GetName() << " (" << std::fixed << std::setprecision(3) << rectPrism->GetLength();
		std::cout << ", " << rectPrism->GetWidth() << ", " << rectPrism->GetHeight() << ")" << std::endl;
		++count;
	}
}

void RectPrism::Load(std::vector<std::string>::iterator &itr)
{
	std::string stName = "";
	double dLength = 0;
	double dWidth = 0;
	double dHeight = 0;
	bool bHasConnection = false;
	int nNumOfEdges = 0;

	//getting members for rectPrism
	stName = (*itr);
	itr++;
	dLength = stod(*itr);
	itr++;
	dWidth = stod(*itr);
	itr++;
	dHeight = stod(*itr);
	itr++;
	bHasConnection = static_cast<bool>(stoi(*itr));
	itr++;

	//constructing a solid box with given length and setting other params
	std::shared_ptr<RectPrism> rectPrism = std::make_shared<RectPrism>(dLength, dWidth, dHeight);
	rectPrism->SetName(stName);

	//getting connectionchannel name
	stName = (*itr);
	itr++;
	rectPrism->GetConnChannel()->SetName(stName);

	//getting and setting members for square planes
	for (auto planePtr : rectPrism->GetConnChannel()->GetSurfaceSet())
	{
		stName = (*itr);
		itr++;
		dHeight = stod(*itr);
		itr++;
		dLength = stod(*itr);
		itr++;
		nNumOfEdges = stoi(*itr);
		itr++;
		// for notes on why using dynamic cast here, refer to load fxn in Sphere
		auto rectPlanePtr = dynamic_cast<RectPlane*>(planePtr.get());
		rectPlanePtr->SetName(stName);
		stName = ""; // resetting name
	}

	RectPrism::m_shapeVec.push_back(rectPrism); // rectPrism object is completed now.
}