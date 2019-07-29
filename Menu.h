#pragma once

#include <string>
#include <iostream>
#include <regex>
#include <iomanip>
#include "Utility.h"
#include "SolidBox.h"
#include "Sphere.h"
#include "RectPlane.h"
#include "CurvedSurface.h"
#include "ConnectionChannel.h"

class Menu {

	friend class Utility;

public:
	void Run()
	{
		std::string strInput = "";
		char cInput = 0;
		std::cout << "Welcome to SolidBox." << std::endl;

		while (tolower(cInput) != 'q')
		{
			WelcomeAndOptions();
			std::regex acceptableInputExpr("^\\s*([1-8|q|Q])\\s*$"); // looking for 1-8 or q or Q                                                         
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);
			cInput = strInput[0]; // making string of length 1 into a single char
			if (cInput == '1')
			{
				CreateSolidBox();
			}
			else if (cInput == '2')
			{
				DeleteExistingSolid();
			}
			else if (cInput == '3')
			{
				ShowSolidsInMemory();
			}
			else if (cInput == '4')
			{
				CopyExistingSolid();
			}
			else if (cInput == '5')
			{
				MoveASolid();
			}
			else if (cInput == '6')
			{
				DebugSolidBox();
			}
			else if (cInput == '7')
			{
				SaveAllObjects();
			}
			else if (cInput == '8')
			{
				LoadAllObjects();
			}
		}
	}

	Menu* GetInstance()
	{
		if (!m_bHasInstance)
		{
			m_instance = new Menu();
		}
		return m_instance;
	}

private:

	Menu* m_instance;
	static bool m_bHasInstance;

	Menu()
	{
		m_bHasInstance = 1;
	}

	~Menu()
	{
		delete m_instance;
		m_bHasInstance = 0;
	}

	void CreateSolidBox()
	{
		std::string strInput = "";
		double dSideLength = 0.0;
		std::regex acceptableInputExpr("^\\s*([0-9]*\\.?[0-9]*)\\s*$"); // looking for a number (if present)
																		// with 0-1 decimals followed by a number (if present) while allowing spaces
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		std::cout << "What would you like the length, width, and height to be? (in cm)" << std::endl;
		std::cout << "ex: 4.5" << std::endl;
		strInput = Utility::GetAndValidateInput(acceptableInputExpr);
		dSideLength = std::stod(strInput); // converting string input into a double

		while (dSideLength == 0.0) // don't want to make a box with sideLength of 0
		{
			std::cout << "Please input a value that is not 0" << std::endl;
			strInput = Utility::GetAndValidateInput(acceptableInputExpr);
			dSideLength = std::stod(strInput);
		}
		std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(dSideLength);
		SolidBox::cubeVec.push_back(box);
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
	}

	void DeleteExistingSolid()
	{
		std::string strInput = "";
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		std::cout << "Type the number corresponding to the cube you wish to delete" << std::endl;
		std::cout << "or press 'b' to go back to the menu." << std::endl;
		std::regex acceptableInputExpr("^\\s*([1-9]+|b|B)\\s*$");
		PrintSolidsInMemory();
		std::cout << std::endl;
		strInput = Utility::InputInVecVal(strInput, acceptableInputExpr);

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}

		auto cubeVecItr = SolidBox::cubeVec.begin() + (stoi(strInput) - 1); // advance iterator by the # given by user 
		(**cubeVecItr).Delete(); // dereference twice (first for iterator, second for shared ptr)
	}

	void ShowSolidsInMemory()
	{
		Utility::PrintNwLnsAndLnDelimiter("-", 55);

		if (SolidBox::cubeVec.size() == 0)
		{
			std::cout << "No solids currently in memory" << std::endl;
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
			return;
		}

		std::string strHeader = "SolidBox name (length of each side in cm)";
		Utility::PrintHeader(strHeader);


		for (auto cubePtr : SolidBox::cubeVec)
		{
			std::cout << cubePtr->name << " (" << std::fixed << std::setprecision(3) << cubePtr->sideLength << ")" << std::endl;
		}

		Utility::PrintNwLnsAndLnDelimiter("-", 55);
	}

	void CopyExistingSolid()
	{
		Utility::PrintNwLnsAndLnDelimiter("-", 55);

		if (SolidBox::cubeVec.size() == 0)
		{
			std::cout << "No solids currently in memory" << std::endl;
			Utility::PrintNwLnsAndLnDelimiter("-", 55);
			return;
		}

		std::cout << "Enter the number corresponding to the cube you wish to copy." << std::endl;
		std::string strInput = "";
		std::regex acceptableInputExpr("^\\s*([0-9]+|b|B)\\s*$"); // looking for the word "cube" followed by
																	// at least one number allowing for leading and trailing spaces
		PrintSolidsInMemory();
		std::cout << std::endl;
		strInput = Utility::InputInVecVal(strInput, acceptableInputExpr);

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}

		auto cubeVecItr = SolidBox::cubeVec.begin();
		cubeVecItr = std::next(cubeVecItr, (stoi(strInput) - 1)); // advance iterator by the # given by user 
		std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>((*cubeVecItr)->sideLength);
		SolidBox::cubeVec.push_back(box);
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
	}

	void MoveASolid()
	{
		std::string strMoveFrom = "";
		std::string strMoveTo = "";

		if (SolidBox::cubeVec.size() < 2)
		{
			std::cout << "There are not enough cubes in memory to make a move." << std::endl;
			return;
		}

		std::cout << "Please enter two numbers corresponding to their cubes separated by the enter key" << std::endl;
		std::cout << "to move one solid to another or press 'b' to go back to main menu." << std::endl;
		std::cout << std::setw(11) << std::left << "(For ex:  1" << std::endl;
		std::cout << std::setw(11) << std::right << "2" << std::endl;
		std::cout << "moves 2 into 1)" << std::endl;
		PrintSolidsInMemory();
		std::cout << std::endl;
		std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want two numbers that can be separated by 
																	// spaces

		//------Get cube selections from user----------------
		//moveFrom cube:
		strMoveFrom = Utility::InputInVecVal(strMoveFrom, acceptableInputExpr);

		if ((strMoveFrom == "b") | (strMoveFrom == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto cubeVecItr_From = SolidBox::cubeVec.begin();
		cubeVecItr_From = std::next(cubeVecItr_From, (stoi(strMoveFrom) - 1)); // find if name given is the name of a cube made 
		//moveTo cube:
		strMoveTo = Utility::InputInVecVal(strMoveTo, acceptableInputExpr);

		if ((strMoveTo == "b") | (strMoveTo == "B")) // user elected to go back to main menu
		{
			return;
		}

		auto cubeVecItr_To = SolidBox::cubeVec.begin();
		cubeVecItr_To = std::next(cubeVecItr_To, (stoi(strMoveTo) - 1)); // find if name given is the name of a cube made 
		//check if trying to move to the same cube
		if (strMoveFrom == strMoveTo)
		{
			std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
			MoveASolid();
		}

		// It's OK to now move From into To
		**cubeVecItr_To = **cubeVecItr_From;
	}

	void DebugSolidBox()
	{
		std::string strInput = "";
		std::regex acceptableInputExpr("^\\s*([0-9]*|b|B)\\s*$"); // want one numbers that can be surrounded by 
																	// whitespace.
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		std::cout << "Type the number corresponding to the desired cube for detailed information" << std::endl;
		std::cout << "or press 'b' to go back to the menu.\n" << std::endl;
		PrintSolidsInMemory();
		std::cout << std::endl;
		strInput = Utility::InputInVecVal(strInput, acceptableInputExpr);

		if ((strInput == "b") || (strInput == "B"))
		{
			return;
		}

		auto cubeVecItr = SolidBox::cubeVec.begin() + (stoi(strInput) - 1); // advance iterator by the # given by user 

		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		PrintDebugInfo(cubeVecItr);
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
	}

	int SaveAllObjects()
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
		outFile << SolidBox::m_nNameIDCounter << ";" << ConnectionChannel<SolidBox>::m_nNameIDCounter << ";" << RectPlane::nameIDCounter << ";";
		outFile << static_cast<int>(SolidBox::cubeVec.size()) << ";\n";
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		std::cout << "Saving...";

		for (auto cubePtr : SolidBox::cubeVec)
		{
			if (count == 0)
			{
				std::cout << cubePtr->name << " (" << std::fixed << std::setprecision(3) << cubePtr->sideLength << ")" << std::endl;
				++count;
			}
			else
			{
				std::cout << std::setw(9) << "" << std::left << cubePtr->name << " (" << std::fixed << std::setprecision(3) << cubePtr->sideLength << ")" << std::endl;
			}

			cubePtr->Save(outFile); // does the actual saving of the objects
		}

		outFile.close();
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
		return 1;
	}

	void LoadAllObjects()
	{
		if (SolidBox::cubeVec.size() != 0)
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
			for (auto element : SolidBox::cubeVec)
			{
				element->channel.Disconnect();
			}
			SolidBox::cubeVec.clear();
		}

		SolidBox::nameIDCounter = 0; // resetting nameIDCounters in case boxes were made
		ConnectionChannel<SolidBox>::nameIDCounter = 0; // and then deleted (making SolidBox::cubeVec.size() == 0 
		RectPlane::nameIDCounter = 0; // and the nameIDCounters != 0)
		LoadASolidBox();
		Utility::PrintNwLnsAndLnDelimiter("-", 55);
	}


	void WelcomeAndOptions()
	{
		std::cout << "What would you like to do?" << std::endl;
		std::cout << "(enter a number or press 'q' to quit)\n\n" << std::endl;
		std::cout << "1)  Create a solid box" << std::endl;
		std::cout << "2)  Delete a solid box" << std::endl;
		std::cout << "3)  Show available solids" << std::endl;
		std::cout << "4)  Copy an existing solid into a new one" << std::endl;
		std::cout << "5)  Move an existing solid's data into another existing solid" << std::endl;
		std::cout << "6)  Debug a solid (print information)" << std::endl;
		std::cout << "7)  Save" << std::endl;
		std::cout << "8)  Load" << std::endl;
	}

	void PrintCubeInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
	{
		std::string strHeader = "Cube:";
		Utility::PrintHeader(strHeader);
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(18) << "SolidBox name:" << (*cubeVecItr)->m_stName << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(18) << "hasConnection:" << (*cubeVecItr)->GetHasConnection() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(18) << "Channel name:" << (*cubeVecItr)->GetConnChannel()->GetConnName() << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(18) << "SideLength (cm):" << std::fixed << std::setprecision(3) << (*cubeVecItr)->GetSideLength() << std::endl;
		std::cout << std::endl;
	}

	void PrintChannelInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
	{
		std::string strHeader = "Channel:";
		Utility::PrintHeader(strHeader);
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(27) << "Channel name:" << (*cubeVecItr)->m_channel.m_stName << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(27) << "Associated SolidBox name:" << (*cubeVecItr)->m_channel.cube->name << std::endl;
		Utility::PrintChar(' ', 5);
		std::cout << std::left << std::setw(27) << "Associated planes' names:" << std::endl;

		for (auto planePtr : (*cubeVecItr)->m_channel.m_surfaceSet)
		{
			Utility::PrintChar(' ', 32);
			std::cout << std::left << planePtr->GetSqPlaneName() << std::endl;
		}

		std::cout << std::endl;
	}

	void PrintPlanesInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
	{
		std::string strHeader = "Planes:";
		Utility::PrintHeader(strHeader);

		for (auto planePtr : (*cubeVecItr)->m_channel.m_surfaceSet)
		{
			Utility::PrintChar(' ', 5);
			std::cout << std::left << std::setw(26) << "Plane name:" << planePtr->GetSqPlaneName() << std::endl;
			Utility::PrintChar(' ', 5);
			std::cout << std::left << std::setw(26) << "Associated channel name:" << planePtr->GetConnChannel()->GetConnName() << std::endl;
			Utility::PrintChar(' ', 5);
			std::cout << std::left << std::setw(26) << "Number of edges:" << planePtr->GetNumOfEdges() << std::endl;
			Utility::PrintChar(' ', 5);
			std::cout << std::left << std::setw(26) << "Length:" << std::fixed << std::setprecision(3) << planePtr->GetSqPlaneLength() << std::endl;
			Utility::PrintChar(' ', 5);
			std::cout << std::left << std::setw(26) << "Height:" << std::fixed << std::setprecision(3) << planePtr->GetSqPlaneHeight() << std::endl;
			std::cout << std::endl;
		}
	}

	void PrintDebugInfo(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
	{
		PrintCubeInfo(cubeVecItr);
		PrintChannelInfo(cubeVecItr);
		PrintPlanesInfo(cubeVecItr);
	}

	void PrintSolidsInMemory()
	{
		int count = 1;
		for (auto cube : SolidBox::shapeVec)
		{
			std::cout << count << ") " << cube->m_stName << std::endl;
			++count;
		}
		count = 1; // resetting for spheres since finished with solid boxes
		for (auto sphere : Sphere::shapeVec)
		{
			std::cout << count << ") " << sphere->m_stName << std::endl;
			++count;
		}
	}

	void DeleteBox(std::vector<std::shared_ptr<SolidBox>>::iterator cubeVecItr)
	{
		cubeVecItr = std::find_if(SolidBox::shapeVec.begin(), SolidBox::shapeVec.end(), [&](std::shared_ptr<SolidBox> box)->bool {return *box == **cubeVecItr; });
		if (cubeVecItr == SolidBox::shapeVec.end())
		{
			std::cout << "Cannot delete solid.  Solid not found" << std::endl;
			return;
		}
		(*cubeVecItr)->m_channel.Disconnect(); // setting planes in planeSet to null
		SolidBox::shapeVec.erase(cubeVecItr); // removing item from vector
	}

	void LoadASolidBox()
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
		ConnectionChannel<SolidBox>::m_nNameIDCounter = connChannelNmIDCntr;
		RectPlane<SolidBox>::m_nNameIDCounter = sqPlnNmIDCntr;
		inFile.close();
	}

	void RetrieveInitialParams(int &solidBoxNameIDCntr, int &connChannelNmIDCntr, int &sqPlnNmIDCntr, int &vecSize, std::vector<std::string>::iterator &itr)
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

};

bool Menu::m_bHasInstance = 0;