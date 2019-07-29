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
#include "RectPrism.h"
#include "RectPlane.h"
#include "CurvedSurface.h"
#include "Utility.h"
//#include "ConnectionChannel.h" // figure out what to do since need to have nameidcounter

class Shape;

const int Menu::PRINTING_WIDTH = 30;

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
	else if (nInput == 3)
	{
		RectPrism::Create();
	}
}

void Menu::DeleteExistingSolid()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$");

	Utility::PrintNwLnsAndLnDelimiter("-", 55);

	if (!Utility::AvailableSolids())
	{
		return;
	}

	std::cout << "Type the number corresponding to the shape you wish to delete" << std::endl;
	std::cout << "or press 'b' to go back to the menu.\n" << std::endl;
	Utility::PrintSolidsWithConseqCntr();
	std::cout << std::endl;
	strInput = Utility::GetAndValidateInput(acceptableInputExpr);

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	nInput = stoi(strInput);

	while ((static_cast<size_t>(nInput < 1)) || 
		(static_cast<size_t>(nInput) > (Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size() + RectPrism::m_shapeVec.size())))
	{
		std::cout << "Selection out of bounds.  Please try again." << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}

		nInput = stoi(strInput);
	}

	// in RectPrism m_shapeVec
	if (static_cast<size_t>(nInput) > (SolidBox::m_shapeVec.size() + Sphere::m_shapeVec.size()))
	{
		int index = nInput;
		index -= (SolidBox::m_shapeVec.size() + Sphere::m_shapeVec.size()); // get index for RectPrism vector
		auto shapeVecItr = RectPrism::m_shapeVec.begin() + index - 1; // make and advance iterator to correct index
		auto shapePtr = dynamic_cast<Shape*>((*shapeVecItr).get());
		(*shapeVecItr)->Delete();
	}
	// in Sphere m_shapeVec
	else if (static_cast<size_t>(nInput) > Sphere::m_shapeVec.size())
	{
		int index = nInput;
		index -= Sphere::m_shapeVec.size();
		auto shapeVecItr = Sphere::m_shapeVec.begin() + index - 1; // make and advance iterator to correct index
		auto shapePtr = dynamic_cast<Shape*>((*shapeVecItr).get());
		(*shapeVecItr)->Delete();
	}
	else
	{
		auto shapeVecItr = SolidBox::m_shapeVec.begin() + nInput - 1;
		auto shapePtr = dynamic_cast<Shape*>((*shapeVecItr).get());
		(*shapeVecItr)->Delete();
	}

	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void Menu::ShowSolidsInMemory()
{
	Utility::PrintNwLnsAndLnDelimiter("-", 55);

	if (!Utility::AvailableSolids())
	{
		return;
	}

	Utility::PrintAllSolids();
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void Menu::CopyExistingSolid()
{
	int numOfSolids = 0; 
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$");
	Utility::PrintNwLnsAndLnDelimiter("-", 55);

	if (!(numOfSolids = Utility::AvailableSolids()))
	{
		return;
	}

	std::cout << "Enter the number corresponding to the shape you wish to copy.\n" << std::endl;
	Utility::PrintSolidsWithConseqCntr();

	do
	{
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}

		nInput = stoi(strInput);

		if ((nInput < 1) || (nInput > numOfSolids))
		{
			std::cout << "Selection out of bounds." << std::endl;
		}

	} while (nInput < 1 || nInput > numOfSolids);

	if (static_cast<size_t>(nInput) <= SolidBox::m_shapeVec.size()) // user chose a SolidBox
	{
		auto shapeVecItr = SolidBox::m_shapeVec.begin();
		shapeVecItr = std::next(shapeVecItr, (nInput - 1)); // advance iterator by the # given by user
		                                                    // (subtract 1 since user sees values starting at 1 and our index starts at 0)
		// make a new SolidBox with same sidelength as selection
		std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>((dynamic_cast<SolidBox*>((*shapeVecItr).get()))->GetSideLength());
		SolidBox::m_shapeVec.push_back(box);
	}
	else if (static_cast<size_t>(nInput) <= (Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size())) // user chose a Sphere. 
	{                                                         // wrote it this way so that more shapes can be easily added after spheres
		auto shapeVecItr = Sphere::m_shapeVec.begin();
		shapeVecItr = std::next(shapeVecItr, (nInput - 1 - SolidBox::m_shapeVec.size())); // advance iterator to spot for Sphere
		// make a new Sphere with same radius as selection
		std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>((dynamic_cast<Sphere*>((*shapeVecItr).get()))->GetRadius());
		Sphere::m_shapeVec.push_back(sphere);
	}
	else if (static_cast<size_t>(nInput) <= (Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size() + RectPrism::m_shapeVec.size()))
	{ // user chose a RectPrism
		auto shapeVecItr = RectPrism::m_shapeVec.begin();
		shapeVecItr = std::next(shapeVecItr, (nInput - 1 - Sphere::m_shapeVec.size() - SolidBox::m_shapeVec.size()));
		// ????????????????????BETTER WAY TO DO THIS????????????????????????
		std::shared_ptr<RectPrism> rectPrism = std::make_shared<RectPrism>((dynamic_cast<RectPrism*>((*shapeVecItr).get()))->GetLength(),
			dynamic_cast<RectPrism*>((*shapeVecItr).get())->GetWidth(), dynamic_cast<RectPrism*>((*shapeVecItr).get())->GetHeight());
		RectPrism::m_shapeVec.push_back(rectPrism);
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

	std::cout << "Number of available cubes:       " << SolidBox::m_shapeVec.size() << " (2 required)" << std::endl;
	std::cout << "Number of available spheres:     " << Sphere::m_shapeVec.size() << " (2 required)" << std::endl;
	std::cout << "Number of available rectPrisms:  " << RectPrism::m_shapeVec.size() << " (2 required)" << std::endl;
	strInput = Utility::SelectShapeType();

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	nInput = stoi(strInput);
	std::cout << "\nPlease enter two numbers corresponding to their shapes separated by the enter key" << std::endl;
	std::cout << "to move one solid to another or press 'b' to go back to main menu." << std::endl;
	std::cout << std::setw(11) << std::left << "(For ex:  1" << std::endl;
	std::cout << std::setw(11) << std::right << "2" << std::endl;
	std::cout << "moves 2 into 1)" << std::endl;

	if (nInput == 1) // user chose SolidBox
	{
		if (SolidBox::m_shapeVec.size() < 2)
		{
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
			std::cout << "There are not enough cubes in memory to make a move." << std::endl;
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
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
		shapeVecItr_To = std::next(shapeVecItr_To, (stoi(strMoveTo) - 1)); // find if name given is the name of a shape made 
		//check if trying to move to the same shape
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
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
			std::cout << "There are not enough spheres in memory to make a move." << std::endl;
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
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
		shapeVecItr_From = std::next(shapeVecItr_From, (stoi(strMoveFrom) - 1)); // find if name given is the name of a shape made 
		//moveTo sphere:
		strMoveTo = Utility::InputInVecVal(strMoveTo, acceptableInputExpr, Sphere::m_shapeVec.size());

		if ((strMoveTo == "b") | (strMoveTo == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto shapeVecItr_To = Sphere::m_shapeVec.begin();
		shapeVecItr_To = std::next(shapeVecItr_To, (stoi(strMoveTo) - 1)); // find if name given is the name of a cube made 
		//check if trying to move to the same shape
		if (strMoveFrom == strMoveTo)
		{
			std::cout << "You cannot move from and to the same sphere.  Please try again." << std::endl;
			Menu* menu = Menu::GetInstance();
			menu->MoveASolid();
		}
		// It's OK to now move From into To
		**shapeVecItr_To = **shapeVecItr_From;
	}
	else if (nInput == 3)
	{
		if (RectPrism::m_shapeVec.size() < 2)
		{
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
			std::cout << "There are not enough rectPrisms in memory to make a move." << std::endl;
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
			return;
		}

		RectPrism::PrintSolids();
		std::cout << std::endl;
		std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want any # or 'b' or 'B' allowing for whitespace

		//------Get RectPrism selections from user----------------
		//moveFrom rectPrism:
		strMoveFrom = Utility::InputInVecVal(strMoveFrom, acceptableInputExpr, RectPrism::m_shapeVec.size());

		if ((strMoveFrom == "b") | (strMoveFrom == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto shapeVecItr_From = RectPrism::m_shapeVec.begin();
		shapeVecItr_From = std::next(shapeVecItr_From, (stoi(strMoveFrom) - 1)); // find if name given is the name of a shape made 
		//moveTo rectPrism:
		strMoveTo = Utility::InputInVecVal(strMoveTo, acceptableInputExpr, RectPrism::m_shapeVec.size());

		if ((strMoveTo == "b") | (strMoveTo == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto shapeVecItr_To = RectPrism::m_shapeVec.begin();
		shapeVecItr_To = std::next(shapeVecItr_To, (stoi(strMoveTo) - 1)); // find if name given is the name of a shape made 
		//check if trying to move to the same shape
		if (strMoveFrom == strMoveTo)
		{
			std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
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

	if (!Utility::AvailableSolids())
	{
		return;
	}

	std::cout << "Type the number corresponding to the desired shape for detailed information" << std::endl;
	std::cout << "or press 'b' to go back to the menu.\n" << std::endl;
	Utility::PrintSolidsWithConseqCntr();
	std::cout << std::endl;
	strInput = Utility::GetAndValidateInput(acceptableInputExpr);

	if ((strInput == "b") || (strInput == "B"))
	{
		return;
	}

	nInput = stoi(strInput);

	while ((static_cast<size_t>(nInput) < 1) || (static_cast<size_t>(nInput) > 
		(Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size() + RectPrism::m_shapeVec.size())))
	{
		std::cout << "Selection out of bounds.  Please try again." << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}

		nInput = stoi(strInput);
	}

	if (static_cast<size_t>(nInput) > (SolidBox::m_shapeVec.size() + Sphere::m_shapeVec.size()))
	{ // user chose a RectPrism
		size_t index = nInput;
		index -= (SolidBox::m_shapeVec.size() + Sphere::m_shapeVec.size()); // get index for RectPrism vector
		auto shapeVecItr = RectPrism::m_shapeVec.begin() + index - 1; // make and advance iterator to correct index
		auto shapePtr = dynamic_cast<Shape*>((*shapeVecItr).get());
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		PrintShapeDebugInfo(shapePtr);
	}
	else if (static_cast<size_t>(nInput) > SolidBox::m_shapeVec.size())
	{ // user chose a Sphere
		size_t index = nInput;
		index -= SolidBox::m_shapeVec.size();
		auto shapeVecItr = Sphere::m_shapeVec.begin() + index - 1; // make and advance iterator to correct index
		auto shapePtr = dynamic_cast<Shape*>((*shapeVecItr).get());
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		PrintShapeDebugInfo(shapePtr);
	}
	else
	{ // user chose a RectPrism
		auto shapeVecItr = SolidBox::m_shapeVec.begin() + nInput - 1;
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

	//if no shape to save, return and don't save
	if (!Utility::IsOkToSave())
	{
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		std::cout << "There are no shapes to save." << std::endl;
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		return 0; // didn't save anything
	}

	// view current files in memory and get input from the user to go to main menu,
	// save a new file, or replace an existing file
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
		std::cout << "Please select the number for the file you wish to overwrite." << std::endl;
		Utility::ViewFiles();
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
	//save all name ID counters for solids, then channel, then surfaces. also save vec sizes on same line 
	outFile << SolidBox::m_nNameIDCounter << ";" << Sphere::m_nNameIDCounter << ";";
	outFile << RectPrism::m_nNameIDCounter << ";" << ConnectionChannel::m_nNameIDCounter << ";";
	outFile << RectPlane::m_nNameIDCounter << ";" << CurvedSurface::m_nNameIDCounter << ";";
	outFile << static_cast<int>(SolidBox::m_shapeVec.size()) << ";" << static_cast<int>(Sphere::m_shapeVec.size()) << ";";
	outFile << static_cast<int>(RectPrism::m_shapeVec.size()) << ";\n";
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Saving...\n\n";

	// save all shapes (cubes then spheres)
	Utility::PrintSolidsWithConseqCntr();

	for (auto cubePtr : SolidBox::m_shapeVec)
	{
		cubePtr->Save(outFile);
	}

	for (auto spherePtr : Sphere::m_shapeVec)
	{
		spherePtr->Save(outFile);
	}

	for (auto rectPrismPtr : RectPrism::m_shapeVec)
	{
		rectPrismPtr->Save(outFile);
	}

	outFile.close();
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	return 1; // successfully saved something
}

void Menu::LoadAllObjects()
{
	Utility::PrintNwLnsAndLnDelimiter("-", 55);

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
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
			return;
		}
		else if (tolower(cInput) == 'y')
		{
			if (!SaveAllObjects())
			{
				Utility::PrintNwLnsAndLnDelimiter("-", 55);
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

		for (auto element : Sphere::m_shapeVec)
		{
			auto elementPtr = dynamic_cast<Sphere*>(element.get());
			elementPtr->GetConnChannel()->Disconnect();
		}

		Sphere::m_shapeVec.clear();

		for (auto element : RectPrism::m_shapeVec)
		{
			auto elementPtr = dynamic_cast<RectPrism*>(element.get());
			elementPtr->GetConnChannel()->Disconnect();
		}

		RectPrism::m_shapeVec.clear();
	}

	// resetting counters in case shapes were made and then deleted (making shapeVec.size() == 0 and nameIDCounters != 0)
	SolidBox::m_nNameIDCounter = 0;
	Sphere::m_nNameIDCounter = 0;
	RectPrism::m_nNameIDCounter = 0;
	ConnectionChannel::m_nNameIDCounter = 0;
	RectPlane::m_nNameIDCounter = 0;
	CurvedSurface::m_nNameIDCounter = 0;
	LoadFile(); // does actual loading of everything
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
	enum ShapeType { cube = 1, sphere, rectPrism };
	std::string shapeType = Utility::GetShapeType(shapePtr);
	std::unordered_map<std::string, ShapeType> stringToEnumMap = { {"cube", cube}, {"sphere", sphere}, {"rectPrism", rectPrism} };
	ShapeType shape = stringToEnumMap[shapeType];
	std::string strHeader = shapeType + ":";

	Utility::PrintHeader(strHeader);
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(PRINTING_WIDTH) << "Shape name:" << shapePtr->GetName() << std::endl;
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(PRINTING_WIDTH) << "hasConnection:" << shapePtr->GetHasConnection() << std::endl;
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(PRINTING_WIDTH) << "Channel name:" << shapePtr->GetConnChannel()->GetName() << std::endl;
	Utility::PrintChar(' ', 5);


	switch (shape)
	{
	case cube:
	{
		auto cubePtr = dynamic_cast<SolidBox*>(shapePtr);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "SideLength (mm):" << std::fixed << std::setprecision(3) << cubePtr->GetSideLength() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Surface Area (mm^2):" << cubePtr->GetSA() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Volume (mm^3):" << cubePtr->GetVol() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::endl;
		break;
	}
	case sphere:
	{
		auto spherePtr = dynamic_cast<Sphere*>(shapePtr);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Radius (mm):" << std::fixed << std::setprecision(3) << spherePtr->GetRadius() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Surface Area (mm^2):" << spherePtr->GetSA() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Volume (mm^3):" << spherePtr->GetVol() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::endl;
		break;
	}
	case rectPrism:
	{
		auto rectPrismPtr = dynamic_cast<RectPrism*>(shapePtr);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Length (mm):" << std::fixed << std::setprecision(3) << rectPrismPtr->GetLength() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Width (mm):" << rectPrismPtr->GetWidth() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Height (mm):" << rectPrismPtr->GetHeight() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Surface Area (mm^2):" << rectPrismPtr->GetSA() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Volume (mm^3):" << rectPrismPtr->GetVol() << std::endl;
		Utility::PrintChar(' ', 5);
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
	bool bIsFirstSurface = true;
	std::string strHeader = "Channel:";

	Utility::PrintHeader(strHeader);
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(PRINTING_WIDTH) << "Channel name:" << shape->GetConnChannel()->GetName() << std::endl;
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(PRINTING_WIDTH) << "Associated Shape name:" << shape->GetConnChannel()->GetShape()->GetName() << std::endl;
	Utility::PrintChar(' ', 5);
	std::cout << std::left << std::setw(PRINTING_WIDTH) << "Associated Surface name(s):";

	for (auto surfacePtr : shape->GetConnChannel()->GetSurfaceSet())
	{
		if (bIsFirstSurface == true)
		{
			std::cout << std::left << surfacePtr->GetName() << std::endl;
			bIsFirstSurface = false;
		}
		else
		{
			Utility::PrintChar(' ', PRINTING_WIDTH + 5);
			std::cout << std::left << surfacePtr->GetName() << std::endl;
		}
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
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Plane name:" << plane->GetName() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Associated channel name:" << plane->GetConnChannel()->GetName() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(PRINTING_WIDTH) << "Number of edges:" << plane->GetNumOfEdges() << std::endl;
		Utility::PrintChar(' ', 5);

		switch (shape)
		{
		case cube:
		{
			auto planePtr = dynamic_cast<RectPlane*>(plane.get());
			std::cout << std::left << std::setw(PRINTING_WIDTH) << "Length:" << std::fixed << std::setprecision(3) << planePtr->GetLength() << std::endl;
			Utility::PrintChar(' ', 5);
			std::cout << std::left << std::setw(PRINTING_WIDTH) << "Height:" << std::fixed << std::setprecision(3) << planePtr->GetHeight() << std::endl;
			std::cout << std::endl;
			break;
		}
		case sphere:
		{
			auto spherePtr = dynamic_cast<CurvedSurface*>(plane.get());
			std::cout << std::left << std::setw(PRINTING_WIDTH) << "Radius (mm):" << std::fixed << std::setprecision(3) << spherePtr->GetRadius() << std::endl;
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

void Menu::LoadFile()
{
	int cubeVecSize = 0;
	int sphereVecSize = 0;
	int nameSize = 0;
	int solidBoxNameIDCntr = 0;
	int sphereNameIDCntr = 0;
	int rectPrismNameIDCntr = 0;
	int connChannelNmIDCntr = 0;
	int rectPlnNmIDCntr = 0;
	int curvedSurfNmIDCntr = 0;
	std::string strInput = "";
	std::string strDataFromFile = "";
	std::vector<std::string> vec;
	std::ifstream inFile;
	std::vector<std::vector<std::string>>::iterator itr;
	std::string strLineData = "";
	std::vector<std::vector<std::string>> vecOfStrVec;
	std::vector<std::string>::iterator innerItr;

	std::cout << "Please type the number corresponding to the file you with to load" << std::endl;
	std::cout << "or press 'b' to go back to the main menu" << std::endl;
	Utility::ViewFiles();
	std::cout << std::endl;
	strInput = Utility::PickFile();

	if (strInput.length() == 0) // user elected to go back to the main menu
	{
		return;
	}

	inFile.open(strInput);

	while (getline(inFile, strLineData))
	{
		vec.clear();
		vec.push_back(strLineData);
		vecOfStrVec.push_back(vec);
	}

	int count = 0;
	std::vector<std::string> myVec;
	itr = vecOfStrVec.begin();

	for (itr = vecOfStrVec.begin(); itr != vecOfStrVec.end(); ++itr)
	{
		innerItr = itr->begin();
		myVec = Utility::TokenizeStringToVec(*innerItr, ';');
		vecOfStrVec[count++] = myVec;
	}

	itr = vecOfStrVec.begin();
	RetrieveInitialParams(solidBoxNameIDCntr, sphereNameIDCntr, rectPrismNameIDCntr, connChannelNmIDCntr, rectPlnNmIDCntr, // don't set NmIDCntrs permanently until
		curvedSurfNmIDCntr, cubeVecSize, sphereVecSize, itr); // finished creating objects since the values will be off due to creation of objects 

	while (itr != vecOfStrVec.end())
	{
		innerItr = itr->begin();

		if (innerItr[0].find("cube") != std::string::npos) // see if the string contains "cube"
			// it should if that line is for a solidbox.  If so, load it.
		{
			SolidBox::Load(innerItr);
		}
		else if (innerItr[0].find("sphere") != std::string::npos) // see if the string contains "sphere".  If so, load it.
		{
			Sphere::Load(innerItr);
		}
		else if (innerItr[0].find("rectPrism") != std::string::npos) // see if string contains "rectPrism".  If so, load it 
		{
			RectPrism::Load(innerItr);
		}
		++itr;
	}

	// ensures counters are the same as when they were saved
	SolidBox::m_nNameIDCounter = solidBoxNameIDCntr;
	Sphere::m_nNameIDCounter = sphereNameIDCntr;
	RectPrism::m_nNameIDCounter = rectPrismNameIDCntr;
	ConnectionChannel::m_nNameIDCounter = connChannelNmIDCntr;
	RectPlane::m_nNameIDCounter = rectPlnNmIDCntr;
	CurvedSurface::m_nNameIDCounter = curvedSurfNmIDCntr;
	inFile.close();
}

void Menu::RetrieveInitialParams(int &solidBoxNameIDCntr, int &sphereNmIDCntr, int &rectPrismNmIDCntr, int &connChannelNmIDCntr,
	int &rectPlnNmIDCntr, int &curvedSurfNmIDCntr, int &cubeVecSize, int &sphereVecSize, std::vector<std::vector<std::string>>::iterator &itr)
{
	std::vector<std::string>::iterator innerItr = itr->begin(); // iterator within each line of the file
	solidBoxNameIDCntr = stoi(*innerItr);
	innerItr++; // increment iterator to go through vec for each param 
	sphereNmIDCntr = stoi(*innerItr);
	innerItr++;
	rectPrismNmIDCntr = stoi(*innerItr);
	innerItr++;
	connChannelNmIDCntr = stoi(*innerItr);
	innerItr++;
	rectPlnNmIDCntr = stoi(*innerItr);
	innerItr++;
	curvedSurfNmIDCntr = stoi(*innerItr);
	++innerItr;
	cubeVecSize = stoi(*innerItr);
	++innerItr;
	sphereVecSize = stoi(*innerItr);
	++itr; // move outter iterator to next position, which should be the next line in the file
}

void Menu::PrintShapeDebugNames()
{
	int count = 0;

	if (SolidBox::m_shapeVec.size() != 0)
	{
		for (auto cube : SolidBox::m_shapeVec)
		{
			std::cout << ++count << ") " << std::setw(18) << cube->GetName() << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
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