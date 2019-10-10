#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "Utility.h"
#include "RectPrism.h"
#include "RectPlane.h"
#include "ConnectionChannel.h"
#include "Menu.h"
#include "Speech.h"

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

// copy constructor
RectPrism::RectPrism(const RectPrism& rectPrism) : Shape(rectPrism)
{
	// don't change name
	if (m_stName.length() == 0)
	{
		m_stName = Utility::CreateUniqueName("rectPrism", m_nNameIDCounter);
	}

	m_nNumOfSurfaces = rectPrism.m_nNumOfSurfaces;
	m_dLength = rectPrism.m_dLength;
	m_dHeight = rectPrism.m_dHeight;
	m_dWidth = rectPrism.m_dWidth;
	m_channel.SetShape(this);
	m_bHasConnection = rectPrism.m_bHasConnection;
	CalcSA();
	CalcVol();
}

RectPrism& RectPrism::operator=(RectPrism &rectPrism)
{
	m_dLength = rectPrism.m_dLength;
	m_dHeight = rectPrism.m_dHeight;
	m_dWidth = rectPrism.m_dWidth;
	m_channel = rectPrism.m_channel;
	m_bHasConnection = rectPrism.m_bHasConnection;
	CalcSA();
	CalcVol();
	rectPrism.Delete(); //**deleting items on right side of = operator**
	return *this;
}

void RectPrism::Delete()
{
	std::vector<std::shared_ptr<RectPrism>>::iterator shapeVecItr = m_shapeVec.begin();
	shapeVecItr = std::find_if(m_shapeVec.begin(), m_shapeVec.end(), [&](std::shared_ptr<RectPrism> shape)->bool {return *shape == *this; });

	if (shapeVecItr == m_shapeVec.end())
	{
		Utility::Display("Cannot delete Shape.  Shape not found.\n");
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
		std::shared_ptr<RectPlane> copy = std::make_shared<RectPlane>(*element);
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
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();
	std::string strInput3 = "";
	std::string strInput2 = "";
	std::string strInput = "";
	double dLength = 0.0;
	double dHeight = 0.0;
	double dWidth = 0.0;
	std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
																	// with 0-1 decimals followed by a number (if present) while allowing spaces


	if (isSpeech)
	{
		Utility::Display("What would you like the length to be? (in mm)\n");
		Utility::Display("ex: 3.3\n");
		// getting length from user
		do
		{
			dLength = Speech::RetrieveDouble();
			if (dLength <= 0.0) Utility::Display("Please input a value that is greater than 0.0\n");
		} while (dLength <= 0.0);

		Utility::Display("What would you like the width to be? (in mm)\n");
		// getting width from user
		do
		{
			dWidth = Speech::RetrieveDouble();
			if (dWidth <= 0.0) Utility::Display("Please input a value that is greater than 0.0\n");
		} while (dWidth <= 0.0);

		Utility::Display("What would you like the height to be? (in mm)\n");
		// getting height from user
		do
		{
			dHeight = Speech::RetrieveDouble();
			if (dHeight <= 0.0) Utility::Display("Please input a value that is greater than 0.0\n");
		} while (dHeight <= 0.0);
	}
	else
	{
		Utility::Display("What would you like the length to be? (in mm)\n");
		Utility::Display("ex: 3.3\n");
		// getting length from user
		do
		{
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);
			dLength = std::stod(strInput); // converting string input into a double
			if (dLength <= 0.0) Utility::Display("Please input a value that is greater than 0.0\n");
		} while (dLength <= 0.0); // don't want to make a box with sideLength of 0

		Utility::Display("What would you like the width to be? (in mm)\n");
		// getting width from user
		do
		{
			strInput3 = Utility::GetAndValidateInput(acceptableInputExpr);
			dWidth = std::stod(strInput3);
			if (dLength <= 0.0) Utility::Display("Please input a value that is greater than 0.0\n");
		} while (dWidth <= 0.0);

		Utility::Display("What would you like the height to be? (in mm)\n");
		// getting height from user
		do
		{
			strInput2 = Utility::GetAndValidateInput(acceptableInputExpr);
			dHeight = std::stod(strInput2);
			if (dHeight <= 0.0) Utility::Display("Please input a value that is greater than 0.0\n");
		} while (dHeight <= 0.0);
	}

	std::shared_ptr<RectPrism> rectPrism = std::make_shared<RectPrism>(dLength, dWidth, dHeight);
	m_shapeVec.push_back(rectPrism);
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

std::string RectPrism::PrintSolids()
{
	std::ostringstream stream;
	int count = 1;

	for (auto rectPrism : m_shapeVec)
	{
		stream << count << ") " << rectPrism->GetName() << " (" << std::fixed << std::setprecision(3) << rectPrism->GetLength();
		stream << ", " << rectPrism->GetWidth() << ", " << rectPrism->GetHeight() << ")" << std::endl;
		++count;
	}

	return stream.str();
}

std::string RectPrism::PrintSolids(int &counter)
{
	std::ostringstream stream;

	for (auto &rectPrism : RectPrism::m_shapeVec)
	{
		stream << counter++ << ") " << rectPrism->GetName() << " (" << std::fixed << std::setprecision(3) << rectPrism->GetLength();
		stream << ", " << rectPrism->GetWidth() << ", " << rectPrism->GetHeight() << ")" << std::endl;
	}

	return stream.str();
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

bool RectPrism::Move()
{
	int nMoveFrom = 0;
	int nMoveTo = 0;
	Menu* menu = Menu::GetInstance();
	bool bIsSpeech = menu->GetIsSpeechFlag();
	int rectPrismVecSize = static_cast<int>(RectPrism::m_shapeVec.size());
	auto shapeVecItr_To = RectPrism::m_shapeVec.begin();
	auto shapeVecItr_From = RectPrism::m_shapeVec.begin();
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' while allowing for whitespace

	Utility::Display(RectPrism::PrintSolids());
	Utility::Display("\n");

	//------Get rectPrism selections from user----------------
	do
	{
		//moveFrom rectPrism:
		do
		{
			if (bIsSpeech)
			{
				Utility::Display("Please select the rectangular prism you are moving FROM or say \"back\" to go to the main menu.\n");
				nMoveFrom = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("Please select the rectangular prism you are moving FROM or press 'b' to go back to the main menu.\n");
				nMoveFrom = Utility::RetrieveVecInput(acceptableInputExpr, rectPrismVecSize);
			}

			if (nMoveFrom == -1) return false; // user elected to go back to main menu

			if ((nMoveFrom < 1) || (nMoveFrom > rectPrismVecSize))
			{
				Utility::Display("Invalid entry.  Please try again.\n");
			}

		} while ((nMoveFrom < 1) || (nMoveFrom > rectPrismVecSize));

		shapeVecItr_From = std::next(shapeVecItr_From, (nMoveFrom - 1));

		//moveTo rectPrism:
		do
		{
			if (bIsSpeech)
			{
				Utility::Display("Please select the rectangular prism you are moving TO or say \"back\" to go to the main menu.\n");
				nMoveTo = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("Please select the rectangular prism you are moving TO or press 'b' to go back to the main menu.\n");
				nMoveTo = Utility::RetrieveVecInput(acceptableInputExpr, rectPrismVecSize);
			}

			if (nMoveTo == -1) return false; // user elected to go back to main menu

			if ((nMoveTo < 1) || (nMoveTo > rectPrismVecSize))
			{
				Utility::Display("Invalid entry.  Please try again.\n");
			}

		} while ((nMoveTo < 1) || (nMoveTo > rectPrismVecSize));

		shapeVecItr_To = std::next(shapeVecItr_To, (nMoveTo - 1));

		if (nMoveFrom == nMoveTo) //check if trying to move to the same shape
		{
			Utility::Display("You cannot move from and to the same rectPrism.  Please try again.\n");
		}

	} while (nMoveFrom == nMoveTo);

	// It's OK to now move From into To
	**shapeVecItr_To = **shapeVecItr_From;
	return true;
}

bool RectPrism::Move(std::shared_ptr<RectPrism> rectPrism)
{
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();
	int nMoveTo = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' while allowing for whitespace
	int rectPrismVecSize = static_cast<int>(RectPrism::m_shapeVec.size());
	auto shapeVecItr_From = std::find(RectPrism::m_shapeVec.begin(), RectPrism::m_shapeVec.end(), rectPrism);
	auto shapeVecItr_To = RectPrism::m_shapeVec.begin();
	do
	{
		//moveTo rectPrism:
		do
		{
			if (isSpeech)
			{
				Utility::Display("Please select the rectPrism you are moving TO or say \"back\" to go to the main menu.\n");
				nMoveTo = Speech::RetrievePosInteger();
			}
			else
			{
				Utility::Display("Please select the rectPrism you are moving TO or press 'b' to go back to the main menu.\n");
				nMoveTo = Utility::RetrieveVecInput(acceptableInputExpr, rectPrismVecSize);
			}

			if (nMoveTo == -1) return false; // user elected to go back to main menu

			if ((nMoveTo < 1) || (nMoveTo > rectPrismVecSize))
			{
				Utility::Display("Invalid entry.  Please try again.\n");
			}

		} while ((nMoveTo < 1) || (nMoveTo > rectPrismVecSize));

		shapeVecItr_To = std::next(shapeVecItr_To, (nMoveTo - 1));

		if (*shapeVecItr_To == *shapeVecItr_From) //check if trying to move to the same shape
		{
			Utility::Display("You cannot move from and to the same rectPrism.  Please try again.\n");
		}

	} while (*shapeVecItr_To == *shapeVecItr_From);

	// It's OK to now move From into To
	**shapeVecItr_To = **shapeVecItr_From;
	return true;
}

std::string RectPrism::PrintShapeInfo()
{
	std::string output = "";
	std::string strHeader = "Rectangular Prism:";
	std::ostringstream stream;

	stream << Utility::PrintHeader(strHeader);
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Shape name:" << m_stName << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "hasConnection:" << m_bHasConnection << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Channel name:" << m_channel.GetName() << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Length (mm):" << std::fixed << std::setprecision(3) << m_dLength << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Width (mm):" << m_dWidth << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Height (mm):" << m_dHeight << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Surface Area (mm^2):" << m_dSurfaceArea << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Volume (mm^3):" << m_dVolume << std::endl;
	stream << Utility::PrintChar(' ', 5);
	stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Number of surfaces:" << m_nNumOfSurfaces << std::endl;
	stream << std::endl;
	return output = stream.str();
}

std::string RectPrism::PrintPlanesInfo()
{
	std::string output = "";
	std::ostringstream stream;

	stream << Utility::PrintHeader("Surfaces:");

	for (auto plane : m_channel.GetSurfaceSet())
	{
		auto planePtr = dynamic_cast<RectPlane*>(plane.get());
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Plane name:" << plane->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Associated shape name:" << plane->GetConnChannel()->GetShape()->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Associated channel name:" << plane->GetConnChannel()->GetName() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Number of edges:" << plane->GetNumOfEdges() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Length:" << std::fixed << std::setprecision(3) << planePtr->GetLength() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Height:" << std::fixed << std::setprecision(3) << planePtr->GetHeight() << std::endl;
		stream << Utility::PrintChar(' ', 5);
		stream << std::left << std::setw(Utility::PRINTING_WIDTH) << "Area:" << std::fixed << std::setprecision(3) << planePtr->GetArea() << std::endl;
		stream << std::endl;
	}

	return output = stream.str();
}