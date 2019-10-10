#pragma once

#include <string>
#include <iostream>
#include <regex>
#include <iomanip>
#include <fstream>
#include <ctype.h>
#include <unordered_map>
#include <sstream>
#include "Menu.h"
#include "SolidBox.h"
#include "Sphere.h"
#include "RectPrism.h"
#include "RectPlane.h"
#include "CurvedSurface.h"
#include "Utility.h"
#include "Speech.h"



class Shape;

bool Menu::GetIsSpeechFlag()
{
	return isSpeech;
}

Menu* Menu::GetInstance(const bool &speechFlag)
{
	static Menu m_instance(speechFlag); // used to help enforce only one instance of Menu
	return &m_instance;
}

void Menu::Run()
{
	bool bValidInput = false;
	int nInput = 0;
	bool bIsSpeech = false;
	bool bQuit = false;
	std::string strInput = "";
	Menu* menu = Menu::GetInstance(bIsSpeech);
	bIsSpeech = menu->GetIsSpeechFlag();
	Utility::Display("Welcome to SolidBox.\n\n");
	std::regex acceptableInputExpr;
	                                                     
	while (bQuit == false)
	{
		menu->WelcomeAndOptions();

		if (bIsSpeech) // for speech input
		{ 
			do
			{
				nInput = Speech::RetrievePosInteger(); // will be -1 if user elected to quit
				
				if (nInput == -1)
				{
					bQuit = true;
					break;
				}
				
			} while (nInput < 1 || nInput > 8);
		}                          
		else // for keyboard input
		{
			acceptableInputExpr = ("^\\s*([1-8|q|Q])\\s*$"); // looking for 1-8 or q or Q    
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);
			
			if (strInput == "q" || strInput == "Q")
			{
				bQuit = true;
				continue;
			}

			nInput = stoi(strInput);
		}

		if (nInput == 1)
		{
			menu->CreateShape();
		}
		else if (nInput == 2)
		{
			menu->DeleteExistingSolid();
		}
		else if (nInput == 3)
		{
			menu->ShowSolidsInMemory();
		}
		else if (nInput == 4)
		{
			menu->CopyExistingSolid();
		}
		else if (nInput == 5)
		{
			menu->MoveASolid();
		}
		else if (nInput == 6)
		{
			menu->DebugSolidBox();
		}
		else if (nInput == 7)
		{
			menu->SaveAllObjects();
		}
		else if (nInput == 8)
		{
			menu->LoadAllObjects();
		}
	}
}

Menu::Menu(int speechFlag) 
{
	if (speechFlag == 1) isSpeech = true;
	else isSpeech = false;
}

Menu::~Menu()
{
	// empty for now
}

// prompt user to select a shape type, validate input, then call appropriate function to shape selection
void Menu::CreateShape()
{
	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
	int nInput = Utility::SelectShapeType();

	if (nInput == -1) return; // user elected to go back to main menu

	if (nInput == 1) SolidBox::Create();
	else if (nInput == 2) Sphere::Create();
	else if (nInput == 3) RectPrism::Create();
}

void Menu::DeleteExistingSolid()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$");
	Menu* menu = Menu::GetInstance();
	bool bSpeechEnabled = menu->GetIsSpeechFlag();
	int cubeVecSize = static_cast<int>(SolidBox::m_shapeVec.size());
	int sphereVecSize = static_cast<int>(Sphere::m_shapeVec.size());
	int rectPrismVecSize = static_cast<int>(RectPrism::m_shapeVec.size());

	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));

	if (!Utility::AvailableSolids())
	{
		Utility::Display("No solids currently in memory\n");
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		return;
	}
	
	if (bSpeechEnabled)
	{
		Utility::Display("Say the number corresponding to the shape you wish to delete\n");
		Utility::Display("or say \"back\" to go back to the menu.\n\n");
		Utility::Display(Utility::PrintSolidsWithConseqCntr());
		Utility::Display("\n");
		nInput = Speech::RetrievePosInteger();
		
		if (nInput == -1) return;

	}
	else
	{
		Utility::Display("Type the number corresponding to the shape you wish to delete\n");
		Utility::Display("or press 'b' to go back to the menu.\n\n");
		Utility::Display(Utility::PrintSolidsWithConseqCntr());
		Utility::Display("\n");
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);

		if ((strInput == "b") || (strInput == "B")) return;
	
		nInput = stoi(strInput);
	}

	while ((nInput < 1) || (nInput > (sphereVecSize + cubeVecSize + rectPrismVecSize)))
	{
		Utility::Display("Selection out of bounds.  Please try again.\n");

		if (bSpeechEnabled)
		{
			nInput = Speech::RetrievePosInteger();

			if (nInput == -1) return;

		}
		else
		{
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);

			if ((strInput == "b") || (strInput == "B")) return;

			nInput = stoi(strInput);
		}
	}

	// in RectPrism m_shapeVec
	if (nInput > (cubeVecSize + sphereVecSize))
	{
		int index = nInput;
		index -= (cubeVecSize + sphereVecSize); // get index for RectPrism vector
		auto shapeVecItr = RectPrism::m_shapeVec.begin() + (index - 1); // make and advance iterator to correct index
		(*shapeVecItr)->Delete();
	}
	// in Sphere m_shapeVec
	else if (nInput > cubeVecSize)
	{
		int index = nInput;
		index -= cubeVecSize;
		auto shapeVecItr = Sphere::m_shapeVec.begin() + (index - 1); // make and advance iterator to correct index
		(*shapeVecItr)->Delete();
	}
	// in SolidBox m_shapeVec
	else
	{
		auto shapeVecItr = SolidBox::m_shapeVec.begin() + (nInput - 1);
		(*shapeVecItr)->Delete();
	}

	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
}

void Menu::ShowSolidsInMemory()
{
	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));

	if (!Utility::AvailableSolids())
	{
		Utility::Display("No solids currently in memory\n");
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		return;
	}

	Utility::Display(Utility::PrintAllSolids());
	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
}

void Menu::CopyExistingSolid()
{
	int numOfSolids = 0; 
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$");

	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));

	if (!(numOfSolids = Utility::AvailableSolids())) // no solids in memory so can't copy
	{
		Utility::Display("Not enough solids in memory to copy.  Need at least 1 solid.\n");
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		return;
	}

	if (isSpeech)
	{
		Utility::Display("Say the number corresponding to the shape you wish to copy.\n\n");
	}
	else
	{
		Utility::Display("Enter the number corresponding to the shape you wish to copy.\n\n");
	}
	
	Utility::Display(Utility::PrintSolidsWithConseqCntr());

	do
	{
		if (isSpeech)
		{
			nInput = Speech::RetrievePosInteger();
			
			if (nInput == -1) return;
		}
		else
		{
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);

			if ((strInput == "b") || (strInput == "B")) return;

			nInput = stoi(strInput);
		}
		
		if ((nInput < 1) || (nInput > numOfSolids)) Utility::Display("Selection out of bounds.\n");

	} while (nInput < 1 || nInput > numOfSolids);

	if (static_cast<size_t>(nInput) <= SolidBox::m_shapeVec.size()) // user chose a SolidBox
	{                                                                 // advance iterator by the # given by user
		auto shapeVecItr = SolidBox::m_shapeVec.begin() + (nInput - 1); // (subtract 1 since user sees values starting at 1 and our index starts at 0)                                               
		// make a new SolidBox with same sidelength as selection
		std::shared_ptr<SolidBox> boxPtr = std::make_shared<SolidBox>(**shapeVecItr);
		SolidBox::m_shapeVec.push_back(boxPtr);
	}
	else if (static_cast<size_t>(nInput) <= (Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size())) // user chose a Sphere. 
	{                                                         // wrote it this way so that more shapes can be easily added after spheres
		auto shapeVecItr = Sphere::m_shapeVec.begin() + (nInput - 1 - SolidBox::m_shapeVec.size()); // advance iterator to spot for Sphere
		// make a new Sphere with same radius as selection
		std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(**shapeVecItr);
		Sphere::m_shapeVec.push_back(sphere);
	}
	else if (static_cast<size_t>(nInput) <= (Sphere::m_shapeVec.size() + SolidBox::m_shapeVec.size() + RectPrism::m_shapeVec.size()))
	{ // user chose a RectPrism
		auto shapeVecItr = RectPrism::m_shapeVec.begin() + (nInput - 1 - Sphere::m_shapeVec.size() - SolidBox::m_shapeVec.size());
		std::shared_ptr<RectPrism> rectPrism = std::make_shared<RectPrism>(**shapeVecItr);
		RectPrism::m_shapeVec.push_back(rectPrism);
	}

	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
}

void Menu::MoveASolid()
{
	std::string strInput = "";
	std::pair<std::string, int> pair;
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$");
	int strMoveFrom = 0;
	int strMoveTo = 0;

	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));

	if ((SolidBox::m_shapeVec.size() < 2) && (Sphere::m_shapeVec.size() < 2) && (RectPrism::m_shapeVec.size() < 2))
	{
		Utility::Display("There are not enough solids in memory to make a move.\n");
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		return;
	}

	pair = Utility::SelectAvailableShape(2);

	if (pair.first == "") return;
	else if (pair.first == "cube") SolidBox::Move(SolidBox::m_shapeVec[pair.second]); // user chose SolidBox 
	else if (pair.first == "sphere") Sphere::Move(Sphere::m_shapeVec[pair.second]); // user chose Sphere
	else if (pair.first == "rectPrism") RectPrism::Move(RectPrism::m_shapeVec[pair.second]); // user chose RectPrism
}

void Menu::DebugSolidBox()
{
	std::string strInput = "";
	int nInput = 0;
	std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // want one number or 'b' or 'B' allowing for whitespace
	int totalShapes = static_cast<int>(SolidBox::m_shapeVec.size() + Sphere::m_shapeVec.size() + RectPrism::m_shapeVec.size());

	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));

	if (!Utility::AvailableSolids())
	{
		Utility::Display("No solids currently in memory\n");
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		return;
	}

	if (isSpeech)
	{
		do
		{
			Utility::Display("Say the number corresponding to the desired shape for detailed information\n");
			Utility::Display("or say \"back\" to go back to the menu.\n\n");
			Utility::Display(Utility::PrintSolidsWithConseqCntr());
			Utility::Display("\n");
			nInput = Speech::RetrievePosInteger();

			if (nInput == -1) return; // user elected to go back to menu

			if ((nInput < 1) || (nInput > totalShapes))
			{
				Utility::Display("Selection out of bounds.  Please try again or say \"back\" to go to the menu.\n");
			}

		} while ((nInput < 1) || (nInput > totalShapes));
	}
	else
	{
		do
		{
			Utility::Display("Type the number corresponding to the desired shape for detailed information\n");
			Utility::Display("or press 'b' to go back to the menu.\n\n");
			Utility::Display(Utility::PrintSolidsWithConseqCntr());
			Utility::Display("\n");
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);

			if ((strInput == "b") || (strInput == "B")) return; // user elected to go back to menu

			nInput = stoi(strInput);

			if ((nInput < 1) || (nInput > totalShapes))
			{
				Utility::Display("Selection out of bounds.  Please try again.\n");
			}

		} while ((nInput < 1) || (nInput > totalShapes));
	}

	if (static_cast<size_t>(nInput) > (SolidBox::m_shapeVec.size() + Sphere::m_shapeVec.size()))
	{ // user chose a RectPrism
		size_t index = nInput;
		index -= (SolidBox::m_shapeVec.size() + Sphere::m_shapeVec.size()); // get index for RectPrism vector
		auto rectPrismVecItr = RectPrism::m_shapeVec.begin() + index - 1; // make and advance iterator to correct index
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		Utility::Display((*rectPrismVecItr)->PrintShapeDebugInfo());
	}
	else if (static_cast<size_t>(nInput) > SolidBox::m_shapeVec.size())
	{ // user chose a Sphere
		size_t index = nInput;
		index -= SolidBox::m_shapeVec.size();
		auto sphereVecItr = Sphere::m_shapeVec.begin() + index - 1; // make and advance iterator to correct index
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		Utility::Display((*sphereVecItr)->PrintShapeDebugInfo());
	}
	else
	{ // user chose a SolidBox
		auto cubeVecItr = SolidBox::m_shapeVec.begin() + nInput - 1;
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		Utility::Display((*cubeVecItr)->PrintShapeDebugInfo());
	}

	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
}

int Menu::SaveAllObjects()
{
	int count = 0;
	char cInput = 0;
	int numOfFiles = 0;
	std::string fileName = "";
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	//if no shape to save, return and don't save
	if (!Utility::IsOkToSave())
	{
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		Utility::Display("There are no shapes to save.\n");
		Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
		return 0; // didn't save anything
	}

	// view current files in memory and get input from the user to go to main menu,
	// save a new file, or replace an existing file
	numOfFiles = Utility::NumOfFilesAvail();
	Utility::Display(Utility::PrintSaveOptions());
	cInput = Utility::SaveOptions();

	if (tolower(cInput) == 'b')
	{
		return 0;
	}
	else if ((tolower(cInput) == 's') && (numOfFiles == 0))
	{
		Utility::Display("Cannot pick an existing file.  No files in memory.\n");
		fileName = Utility::PickNewFile();
	}
	else if (tolower(cInput) == 's')
	{
		Utility::Display("Please select the number for the file you wish to overwrite.\n");
		Utility::Display(Utility::ViewFiles());
		fileName = Utility::PickFile();
		
		if (fileName == "b") return 0; // user elected to go back to the main menu
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
	Utility::Display("Saving...\n\n");
	Utility::Display(Utility::PrintSolidsWithConseqCntr());
	// save all shapes
	menu->SaveShapes(outFile);
	outFile.close();
	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
	return 1; // successfully saved something
}

void Menu::LoadAllObjects()
{
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();

	Utility::PrintNwLnsAndLnDelimiter("-", 55);

	if (SolidBox::m_shapeVec.size() != 0 || Sphere::m_shapeVec.size() != 0 || RectPrism::m_shapeVec.size() != 0)
	{
		std::string strInput = "";
		char cInput = 0;
		std::regex acceptableInputExpr("^\\s*([bByYnN])\\s*$"); // looking for single character that is b, B, y, Y, n, or N
		Menu* menu = Menu::GetInstance();
		bool isSpeech = menu->GetIsSpeechFlag();

		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		Utility::Display("Do you want to save your current data (will be overwritten) before loading a file?\n");
		Utility::Display(Utility::PrintLoadOptions());
		cInput = Utility::LoadOptions();

		if (tolower(cInput) == 'b')
		{
			Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
			return;
		}
		else if (tolower(cInput) == 'y')
		{
			if (!SaveAllObjects())
			{
				Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
				return;
			}

		}
		else if (tolower(cInput) == 'n') {} //do nothing here since deleting and loading data will occur below

		// deleting old data
		Utility::DeleteAllData();
	}

	LoadFile(); // does actual loading of everything
	Utility::Display(Utility::PrintNwLnsAndLnDelimiter("-", 55));
}

void Menu::WelcomeAndOptions()
{
	Utility::Display("What would you like to do?\n");
	
	if (isSpeech)
	{
		Utility::Display("(say a number or say \"quit\" to quit)\n\n\n");
	}
	else
	{
		Utility::Display("(enter a number or press 'q' to quit)\n\n\n");
	}
	
	Utility::Display("1)  Create a shape\n");
	Utility::Display("2)  Delete a solid box\n");
	Utility::Display("3)  Show available solids\n");
	Utility::Display("4)  Copy an existing solid into a new one\n");
	Utility::Display("5)  Move an existing solid's data into another existing solid\n");
	Utility::Display("6)  Debug a solid (print information)\n");
	Utility::Display("7)  Save\n");
	Utility::Display("8)  Load\n");
	Utility::Display("\n");
}

void Menu::LoadFile()
{
	Menu* menu = Menu::GetInstance();
	bool isSpeech = menu->GetIsSpeechFlag();
	int cubeVecSize = 0;
	int sphereVecSize = 0;
	int nameSize = 0;
	int solidBoxNameIDCntr = 0;
	int sphereNameIDCntr = 0;
	int rectPrismNameIDCntr = 0;
	int connChannelNmIDCntr = 0;
	int rectPlnNmIDCntr = 0;
	int curvedSurfNmIDCntr = 0;
	int count = 0;
	std::string strInput = "";
	std::string strDataFromFile = "";
	std::vector<std::string> myVec;
	std::vector<std::string> vec;
	std::ifstream inFile;
	std::vector<std::vector<std::string>>::iterator itr;
	std::string strLineData = "";
	std::vector<std::vector<std::string>> vecOfStrVec;
	std::vector<std::string>::iterator innerItr;

	if (isSpeech)
	{
		Utility::Display("Please say the number corresponding to the file you wish to load\n");
		Utility::Display("or say \"back\" to go to the main menu.\n");
	}
	else
	{
		Utility::Display("Please type the number corresponding to the file you wsth to load\n");
		Utility::Display("or press 'b' to go back to the main menu.\n");
	}
	
	Utility::Display(Utility::ViewFiles());
	Utility::Display("\n");
	strInput = Utility::PickFile();

	if (strInput.length() == 0) return; // user elected to go back to the main menu

	inFile.open(strInput);

	while (getline(inFile, strLineData))
	{
		vec.clear();
		vec.push_back(strLineData);
		vecOfStrVec.push_back(vec);
	}
	
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

std::string Menu::PrintShapeDebugNames()
{
	std::string output = "";
	std::ostringstream stream;
	int count = 0;

	if (SolidBox::m_shapeVec.size() != 0)
	{
		for (auto cube : SolidBox::m_shapeVec)
		{
			stream << ++count << ") " << std::setw(18) << cube->GetName() << " (" << std::fixed << std::setprecision(3) << cube->GetSideLength() << ")" << std::endl;
		}
	}

	if (Sphere::m_shapeVec.size() != 0)
	{
		for (auto sphere : Sphere::m_shapeVec)
		{
			stream << ++count << ") " << sphere->GetName() << " (" << std::fixed << std::setprecision(3) << sphere->GetRadius() << ")" << std::endl;
		}
	}

	if (RectPrism::m_shapeVec.size() != 0)
	{
		for (auto rectPrism : RectPrism::m_shapeVec)
		{
			stream << ++count << ") " << rectPrism->GetName() << " (";
			stream << std::fixed << std::setprecision(3) << rectPrism->GetLength() << ", " << rectPrism->GetHeight() << ")" << std::endl;
		}
	}
	stream << std::endl;
	return output = stream.str();
}

void Menu::SaveShapes(std::ofstream &outFile)
{
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
}