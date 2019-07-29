#pragma once

#include <string>
#include <iostream>
#include <regex>
#include <iomanip>
#include <fstream>
#include <ctype.h>
#include <unordered_map>
#include "Menu.h"
#include "SolidBox.h"
#include "Sphere.h"
#include "RectPlane.h"
#include "CurvedSurface.h"
#include "Utility.h"
//#include "ConnectionChannel.h" // figure out what to do since need to have nameidcounter

class Shape;


Menu* Menu::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Menu();
	}
	return m_instance;
}

void Menu::Run()
{
	std::string strInput = "";
	char cInput = 0;
	Menu* menu = Menu::GetInstance();
	std::cout << "Welcome to SolidBox." << std::endl;

	while (tolower(cInput) != 'q')
	{
		menu->WelcomeAndOptions();
		std::regex acceptableInputExpr("^\\s*([1-8|q|Q])\\s*$"); // looking for 1-8 or q or Q                                                         
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		cInput = strInput[0]; // making string of length 1 into a single char
		if (cInput == '1')
		{
			menu->CreateShape();
		}
		else if (cInput == '2')
		{
			menu->DeleteExistingSolid();
		}
		else if (cInput == '3')
		{
			menu->ShowSolidsInMemory();
		}
		else if (cInput == '4')
		{
			menu->CopyExistingSolid();
		}
		else if (cInput == '5')
		{
			menu->MoveASolid();
		}
		else if (cInput == '6')
		{
			menu->DebugSolidBox();
		}
		else if (cInput == '7')
		{
			menu->SaveAllObjects();
		}
		else if (cInput == '8')
		{
			menu->LoadAllObjects();
		}
	}
}

Menu::Menu()
{
	// empty for now
}

Menu::~Menu()
{
	delete m_instance;
	m_instance = nullptr; // resetting for good practices
}

// prompt user to select a shape type, validate input, then call appropriate function to shape selection
void Menu::CreateShape()
{
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	std::string strInput = Utility::SelectShapeType();
	int nInput = 0;

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	nInput = stoi(strInput);

	if (nInput == 1)
	{
		SolidBox::Create();
	}
	else if (nInput == 2)
	{
		Sphere::Create();
	}
}

void Menu::DeleteExistingSolid()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$");

	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	strInput = Utility::SelectShapeType();

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	nInput = stoi(strInput);

	if (nInput == 1)
	{
		std::cout << "Type the number corresponding to the cube you wish to delete" << std::endl;
		std::cout << "or press 'b' to go back to the menu." << std::endl;
		Utility::PrintAllSolids();
		std::cout << std::endl;
		strInput = Utility::InputInVecVal(strInput, acceptableInputExpr, SolidBox::m_shapeVec.size());

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}
		auto shapeVecItr = SolidBox::m_shapeVec.begin() + (stoi(strInput) - 1); // advance iterator by the # given by the user
		(**shapeVecItr).Delete();
	}
	else if (nInput == 2)
	{
		std::cout << "Type the number corresponding to the sphere you wish to delete" << std::endl;
		std::cout << "or press 'b' to go back to the menu." << std::endl;
		Utility::PrintAllSolids();
		std::cout << std::endl;
		strInput = Utility::InputInVecVal(strInput, acceptableInputExpr, Sphere::m_shapeVec.size());

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}
		auto shapeVecItr = Sphere::m_shapeVec.begin() + (stoi(strInput) - 1); // advance iterator by the # given by the user
		(*shapeVecItr)->Delete();
	}
}

void Menu::ShowSolidsInMemory()
{
	Utility::PrintNwLnsAndLnDelimiter("-", 55);

	if ((SolidBox::m_shapeVec.size() == 0) && (Sphere::m_shapeVec.size() == 0))
	{
		std::cout << "No solids currently in memory" << std::endl;
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		return;
	}

	Utility::PrintAllSolids();
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void Menu::CopyExistingSolid()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$");
	Utility::PrintNwLnsAndLnDelimiter("-", 55);

	if ((SolidBox::m_shapeVec.size() == 0) && (Sphere::m_shapeVec.size() == 0))
	{
		std::cout << "No solids currently in memory" << std::endl;
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		return;
	}

	strInput = Utility::SelectShapeType();

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	nInput = stoi(strInput);

	if (nInput == 1) // user chose SolidBox
	{
		std::cout << "Enter the number corresponding to the cube you wish to copy." << std::endl;
		std::string strInput = "";
		std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // looking for any number or 'b' or 'B'
																	// allowing for leading and trailing whitespaces
		SolidBox::PrintSolids();
		std::cout << std::endl;
		strInput = Utility::InputInVecVal(strInput, acceptableInputExpr, SolidBox::m_shapeVec.size());

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}

		auto shapeVecItr = SolidBox::m_shapeVec.begin();
		shapeVecItr = std::next(shapeVecItr, (stoi(strInput) - 1)); // advance iterator by the # given by user 
		std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>((dynamic_cast<SolidBox*>((*shapeVecItr).get()))->GetSideLength());
		SolidBox::m_shapeVec.push_back(box);
	}
	else if (nInput == 2) // user chose Sphere
	{
		std::cout << "Enter the number corresponding to the sphere you wish to copy." << std::endl;
		std::string strInput = "";
		std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // looking for same parameters as nInput == 1
		Sphere::PrintSolids();
		std::cout << std::endl;
		strInput = Utility::InputInVecVal(strInput, acceptableInputExpr, Sphere::m_shapeVec.size());

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}

		auto shapeVecItr = Sphere::m_shapeVec.begin();
		shapeVecItr = std::next(shapeVecItr, (stoi(strInput) - 1)); // advance iterator by the # given by user 
		std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>((dynamic_cast<Sphere*>((*shapeVecItr).get()))->GetRadius());
		Sphere::m_shapeVec.push_back(sphere);
	}
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void Menu::MoveASolid()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$");
	std::string strMoveFrom = "";
	std::string strMoveTo = "";

	Utility::PrintNwLnsAndLnDelimiter("-", 55);

	if ((SolidBox::m_shapeVec.size() < 2) && (Sphere::m_shapeVec.size() < 2))
	{
		std::cout << "There are not enough solids in memory to make a move." << std::endl;
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		return;
	}

	strInput = Utility::SelectShapeType();

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	nInput = stoi(strInput);
	std::cout << "Please enter two numbers corresponding to their shapes separated by the enter key" << std::endl;
	std::cout << "to move one solid to another or press 'b' to go back to main menu." << std::endl;
	std::cout << std::setw(11) << std::left << "(For ex:  1" << std::endl;
	std::cout << std::setw(11) << std::right << "2" << std::endl;
	std::cout << "moves 2 into 1)" << std::endl;

	if (nInput == 1) // user chose SolidBox
	{
		if (SolidBox::m_shapeVec.size() < 2)
		{
			std::cout << "There are not enough cubes in memory to make a move." << std::endl;
			return;
		}

		SolidBox::PrintSolids();
		std::cout << std::endl;
		std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' allowing for whitespace

		//------Get cube selections from user----------------
		//moveFrom cube:
		strMoveFrom = Utility::InputInVecVal(strMoveFrom, acceptableInputExpr, SolidBox::m_shapeVec.size());

		if ((strMoveFrom == "b") | (strMoveFrom == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto shapeVecItr_From = SolidBox::m_shapeVec.begin();
		shapeVecItr_From = std::next(shapeVecItr_From, (stoi(strMoveFrom) - 1)); // find if name given is the name of a cube made 
		//moveTo cube:
		strMoveTo = Utility::InputInVecVal(strMoveTo, acceptableInputExpr, SolidBox::m_shapeVec.size());

		if ((strMoveTo == "b") | (strMoveTo == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto shapeVecItr_To = SolidBox::m_shapeVec.begin();
		shapeVecItr_To = std::next(shapeVecItr_To, (stoi(strMoveTo) - 1)); // find if name given is the name of a cube made 
		//check if trying to move to the same cube
		if (strMoveFrom == strMoveTo)
		{
			std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
			Menu* menu = Menu::GetInstance();
			menu->MoveASolid();
		}

		// It's OK to now move From into To
		**shapeVecItr_To = **shapeVecItr_From;
	}
	else if (nInput == 2)
	{
		if (Sphere::m_shapeVec.size() < 2)
		{
			std::cout << "There are not enough spheres in memory to make a move." << std::endl;
			return;
		}

		Sphere::PrintSolids();
		std::cout << std::endl;
		std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' allowing for whitespace

		//------Get sphere selections from user----------------
		//moveFrom sphere:
		strMoveFrom = Utility::InputInVecVal(strMoveFrom, acceptableInputExpr, Sphere::m_shapeVec.size());

		if ((strMoveFrom == "b") | (strMoveFrom == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto shapeVecItr_From = Sphere::m_shapeVec.begin();
		shapeVecItr_From = std::next(shapeVecItr_From, (stoi(strMoveFrom) - 1)); // find if name given is the name of a cube made 
		//moveTo sphere:
		strMoveTo = Utility::InputInVecVal(strMoveTo, acceptableInputExpr, Sphere::m_shapeVec.size());

		if ((strMoveTo == "b") | (strMoveTo == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto shapeVecItr_To = Sphere::m_shapeVec.begin();
		shapeVecItr_To = std::next(shapeVecItr_To, (stoi(strMoveTo) - 1)); // find if name given is the name of a cube made 
		//check if trying to move to the same cube
		if (strMoveFrom == strMoveTo)
		{
			std::cout << "You cannot move from and to the same sphere.  Please try again." << std::endl;
			Menu* menu = Menu::GetInstance();
			menu->MoveASolid();
		}
		// It's OK to now move From into To
		**shapeVecItr_To = **shapeVecItr_From;
	}
}

void Menu::DebugSolidBox()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // want one number or 'b' or 'B' allowing for whitespace

	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Type the number corresponding to the desired shape for detailed information" << std::endl;
	std::cout << "or press 'b' to go back to the menu.\n" << std::endl;
	PrintShapeDebugNames();
	strInput = Utility::GetAndValidateInput(acceptableInputExpr);

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	SolidBox::PrintSolids();
	Sphere::PrintSolids();
	std::cout << std::endl;
	nInput = stoi(strInput);

	if ((Sphere::m_shapeVec.size() == 0) && (SolidBox::m_shapeVec.size() == 0))
	{
		std::cout << "No solids currently in memory" << std::endl;
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		return;
	}

	if (nInput > (Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size()))
	{
		std::cout << "Selection out of bounds.  Please try again." << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
	}

	while ((nInput < 1) || (nInput > (Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size())))
	{
		std::cout << "Selection out of bounds.  Please try again." << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		
	}

	if (nInput > SolidBox::m_shapeVec.size())
	{
		int index = nInput;
		index -= SolidBox::m_shapeVec.size(); // get index for Sphere vector
		auto shapeVecItr = Sphere::m_shapeVec.begin() + index - 1; // make and advance iterator to correct index
		auto shapePtr = dynamic_cast<Shape*>((*shapeVecItr).get());
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		PrintShapeDebugInfo(shapePtr);
	}
	else
	{
		auto shapeVecItr = SolidBox::m_shapeVec.begin() + nInput - 1; // make and advance iterator to correct index
		auto shapePtr = dynamic_cast<Shape*>((*shapeVecItr).get());
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		PrintShapeDebugInfo(shapePtr);
	}

	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

int Menu::SaveAllObjects()
{
	int count = 0;
	char cInput = 0;
	int numOfFiles = 0;
	std::string fileName = "";
	//if no boxes to save, return and don't save
	if (!Utility::IsOkToSave())
	{
		std::cout << "There are no solid boxes to save." << std::endl;
		return 0;
	}

	// view current files in memory and get input from the user to go to main menu,
	// save a new file, or replace an existing file
	Utility::ViewFiles();
	numOfFiles = Utility::NumOfFilesAvail();
	cInput = Utility::SaveOptions();

	if (tolower(cInput) == 'b')
	{
		return 0;
	}
	else if ((tolower(cInput) == 's') && (numOfFiles == 0))
	{
		std::cout << "Cannot pick an existing file.  No files in memory." << std::endl;
		fileName = Utility::PickNewFile();
	}
	else if (tolower(cInput) == 's')
	{
		fileName = Utility::PickFile();
		if (fileName.length() == 0) // user elected to go back to the main menu
		{
			return 0;
		}
	}
	else if (cInput == 'n')
	{
		fileName = Utility::PickNewFile();
	}
	// saving objects to file now
	std::ofstream outFile;
	outFile.open(fileName);
	outFile << SolidBox::m_nNameIDCounter << ";" /*<< ConnectionChannel<SolidBox, RectPlane<SolidBox>>::m_nNameIDCounter */ << ";" << RectPlane::m_nNameIDCounter << ";";
	outFile << static_cast<int>(SolidBox::m_shapeVec.size()) << ";\n";
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Saving...";

	for (auto cubePtr : SolidBox::m_shapeVec)
	{
		if (count == 0)
		{
			std::cout << cubePtr->GetName() << " (" << std::fixed << std::setprecision(3) << cubePtr->GetSideLength() << ")" << std::endl;
			++count;
		}
		else
		{
			std::cout << std::setw(9) << "" << std::left << cubePtr->GetName() << " (" << std::fixed << std::setprecision(3) << cubePtr->GetSideLength() << ")" << std::endl;
		}

		cubePtr->Save(outFile); // does the actual saving of the objects
	}

	outFile.close();
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	return 1;
}

void Menu::LoadAllObjects()
{
	if (SolidBox::m_shapeVec.size() != 0)
	{
		std::string strInput = "";
		char cInput = 0;
		std::regex acceptableInputExpr("^\\s*([bByYnN])\\s*$"); // looking for single character that is b, B, y, Y, n, or N
		std::cout << "Do you want to save your current data (will be overwritten) before loading a file?" << std::endl;
		std::cout << "Press 'y' to save your data before loading, 'n' to overwrite the current data with a file," << std::endl;
		std::cout << "or 'b' to go back to the main menu" << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		cInput = strInput[0];

		if (tolower(cInput) == 'b')
		{
			return;
		}
		else if (tolower(cInput) == 'y')
		{
			if (!SaveAllObjects())
			{
				return;
			}

		}
		else if (tolower(cInput) == 'n')
		{
			//do nothing here since deleting and loading data will occur below
		}

		// deleting old data
		for (auto element : SolidBox::m_shapeVec)
		{
			auto elementPtr = dynamic_cast<SolidBox*>(element.get());
			elementPtr->GetConnChannel()->Disconnect();
		}
		SolidBox::m_shapeVec.clear();
	}

	SolidBox::m_nNameIDCounter = 0; // resetting nameIDCounters in case boxes were made
	//ConnectionChannel<SolidBox, RectPlane<SolidBox>>::m_nNameIDCounter = 0; // and then deleted (making SolidBox::cubeVec.size() == 0 
	RectPlane::m_nNameIDCounter = 0; // and the nameIDCounters != 0)
	LoadASolidBox();
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}


void Menu::WelcomeAndOptions()
{
	std::cout << "What would you like to do?" << std::endl;
	std::cout << "(enter a number or press 'q' to quit)\n\n" << std::endl;
	std::cout << "1)  Create a shape" << std::endl;
	std::cout << "2)  Delete a solid box" << std::endl;
	std::cout << "3)  Show available solids" << std::endl;
	std::cout << "4)  Copy an existing solid into a new one" << std::endl;
	std::cout << "5)  Move an existing solid's data into another existing solid" << std::endl;
	std::cout << "6)  Debug a solid (print information)" << std::endl;
	std::cout << "7)  Save" << std::endl;
	std::cout << "8)  Load" << std::endl;
	std::cout << std::endl;
}

void Menu::PrintShapeInfo(Shape* shapePtr)
{
	enum ShapeType { cube, sphere };
	std::string shapeType = Utility::GetShapeType(shapePtr);
	std::unordered_map<std::string, ShapeType> stringToEnumMap = { {"cube", cube}, {"sphere", sphere} };
	ShapeType shape = stringToEnumMap[shapeType];
	std::string strHeader = shapeType + ":";

	Utility::PrintHeader(strHeader);
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "Shape name:" << shapePtr->GetName() << std::endl;
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "hasConnection:" << shapePtr->GetHasConnection() << std::endl;
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(18) << "Channel name:" << shapePtr->GetConnChannel()->GetName() << std::endl;
	Utility::PrintChar(' ', 5);

	switch (shape)
	{
	case cube:
	{
		auto cubePtr = dynamic_cast<SolidBox*>(shapePtr);
		std::cout << std::left << std::setw(18) << "SideLength (mm):" << std::fixed << std::setprecision(3) << cubePtr->GetSideLength() << std::endl;
		std::cout << std::endl;
		break;
	}
	case sphere:
	{
		auto spherePtr = dynamic_cast<Sphere*>(shapePtr);
		std::cout << std::left << std::setw(18) << "Radius (mm):" << std::fixed << std::setprecision(3) << spherePtr->GetRadius() << std::endl;
		std::cout << std::endl;
		break;
	}
	default:
	{
		std::cout << "Not a valid shape" << std::endl;
		break;
	}
	}
}

void Menu::PrintChannelInfo(Shape* shape)
{
	int counter = 0;
	bool isCube = false;
	bool isSphere = false;
	/*std::string name = shape->GetConnChannel()->GetName();
	std::string namePrefix = Utility::GetShapeType(shape);
	auto cubePtr = dynamic_cast<SolidBox*>(shape);*/
	std::string strHeader = "Channel:";

	Utility::PrintHeader(strHeader);
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Channel name:" << shape->GetConnChannel()->GetName() << std::endl;
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Associated Shape name:" << shape->GetConnChannel()->GetShape()->GetName() << std::endl;
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(27) << "Associated Surface name(s):" << std::endl;

	for (auto surfacePtr : shape->GetConnChannel()->GetSurfaceSet())
	{
		Utility::PrintChar(' ', 32);
		std::cout << std::left << surfacePtr->GetName() << std::endl;
	}

	std::cout << std::endl;
}

void Menu::PrintPlanesInfo(Shape* shapePtr)
{
	enum ShapeType { cube, sphere };
	std::string shapeType = Utility::GetShapeType(shapePtr);
	std::unordered_map<std::string, ShapeType> stringToEnumMap = { {"cube", cube}, {"sphere", sphere} };
	ShapeType shape = stringToEnumMap[shapeType];

	Utility::PrintHeader("Surfaces");

	for (auto plane : shapePtr->GetConnChannel()->GetSurfaceSet())
	{
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(26) << "Plane name:" << plane->GetName() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(26) << "Associated channel name:" << plane->GetConnChannel()->GetName() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(26) << "Number of edges:" << plane->GetNumOfEdges() << std::endl;
		Utility::PrintChar(' ', 5);

		switch (shape)
		{
		case cube:
		{
			auto planePtr = dynamic_cast<RectPlane*>(plane.get());
			std::cout << std::left << std::setw(26) << "Length:" << std::fixed << std::setprecision(3) << planePtr->GetSqPlaneLength() << std::endl;
			Utility::PrintChar(' ', 5);
			std::cout << std::left << std::setw(26) << "Height:" << std::fixed << std::setprecision(3) << planePtr->GetSqPlaneHeight() << std::endl;
			std::cout << std::endl;
			break;
		}
		case sphere:
		{
			auto spherePtr = dynamic_cast<CurvedSurface*>(plane.get());
			std::cout << std::left << std::setw(18) << "Radius (mm):" << std::fixed << std::setprecision(3) << spherePtr->GetRadius() << std::endl;
			std::cout << std::endl;
			break;
		}
		default:
		{
			std::cout << "Not a valid shape" << std::endl;
			break;
		}
		}
	}
}

void Menu::PrintShapeDebugInfo(Shape* shape)
{
	PrintShapeInfo(shape);
	PrintChannelInfo(shape);
	PrintPlanesInfo(shape);
}

void Menu::LoadASolidBox()
{
	int vecSize = 0;
	int nameSize = 0;
	int solidBoxNameIDCntr = 0;
	int connChannelNmIDCntr = 0;
	int sqPlnNmIDCntr = 0;
	std::string strInput = "";
	std::string strDataFromFile = "";
	std::vector<std::string> vec;
	std::ifstream inFile;
	std::vector<std::string>::iterator itr;
	std::string strLineData = "";

	Utility::ViewFiles();
	std::cout << "Please type the number corresponding to the file you with to load" << std::endl;
	std::cout << "or press 'b' to go back to the main menu" << std::endl;
	strInput = Utility::PickFile();

	if (strInput.length() == 0) // user elected to go back to the main menu
	{
		return;
	}

	inFile.open(strInput);

	while (getline(inFile, strLineData))
	{
		strDataFromFile += strLineData + ":;";
	}

	vec = Utility::TokenizeStringToVec(strDataFromFile, ';');
	std::cout << "items in vec:  " << std::endl;
	for (auto element : vec)
	{
		std::cout << element << std::endl;
	}
	itr = vec.begin();
	RetrieveInitialParams(solidBoxNameIDCntr, connChannelNmIDCntr, sqPlnNmIDCntr, vecSize, itr); // don't set NmIDCntrs permanently until
														// finished creating objects since the values will be off due to creation of objects 
	while (itr != vec.end())
	{

		if ((*itr).find("cube") != std::string::npos) // see if the string contains "cube"
			// it should if that line is for a solidbox.  If so, load it.
		{
			SolidBox::Load(itr, vecSize);
		}
	}

	// ensures counters are the same as when they were saved
	SolidBox::m_nNameIDCounter = solidBoxNameIDCntr;
	//ConnectionChannel<SolidBox, RectPlane<SolidBox>>::m_nNameIDCounter = connChannelNmIDCntr;
	RectPlane::m_nNameIDCounter = sqPlnNmIDCntr;
	inFile.close();
}

void Menu::RetrieveInitialParams(int &solidBoxNameIDCntr, int &connChannelNmIDCntr, int &sqPlnNmIDCntr, int &vecSize, std::vector<std::string>::iterator &itr)
{
	std::cout << "solidboxnameidcntr:  " << (*itr) << std::endl;
	solidBoxNameIDCntr = stoi(*itr);
	itr++; // increment iterator to go through vec for each param
	std::cout << "connchannelnmidcntr:  " << (*itr) << std::endl;
	connChannelNmIDCntr = stoi(*itr);
	itr++;
	std::cout << "sqplnnmidcntr:  " << (*itr) << std::endl;
	sqPlnNmIDCntr = stoi(*itr);
	itr++;
	std::cout << "vecsize:  " << (*itr) << std::endl;
	vecSize = stoi(*itr);
	itr++; std::cout << "in retrieve initial params. should be ':' " << (*itr) << std::endl;
	itr++; // move past ":" delimiter
}

void Menu::PrintShapeDebugNames()
{
	int count = 0;

	if (SolidBox::m_shapeVec.size() != 0)
	{
		for (auto cube : SolidBox::m_shapeVec)
		{
			std::cout << ++count << ") " << cube->GetName() << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
		}
	}

	if (Sphere::m_shapeVec.size() != 0)
	{
		for (auto sphere : Sphere::m_shapeVec)
		{
			std::cout << ++count << ") " << sphere->GetName() << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
		}
	}
	std::cout << std::endl;
}

Menu* Menu::m_instance = nullptr;