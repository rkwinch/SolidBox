#include <iostream>
#include <limits>
#include <string>
#include <regex>
#include <ctype.h>
#include <set>
#include <iomanip>
#include <memory>
#include <afx.h>
#include <afxwin.h>
#include <algorithm>
#include <vector>
#include <Windows.h>
#include <fstream>
#include "ConnectionChannel.h"
#include "SquarePlane.h"
#include "SolidBox.h"
#include "Utility.h"
#include <chrono>
#include <map>

#include <sapi.h>
#include <sphelper.h>
#include <locale>
#include <sstream>

const ULONGLONG grammarId = 0;
const wchar_t* ruleName1 = L"ruleName1";

std::string start_listening();
ISpRecoGrammar* init_grammar(ISpRecoContext* recoContext);
std::string get_text(ISpRecoContext* reco_context);
std::map<std::string, int> numberInputs;
void WelcomeAndOptions();
void CreateSolidBox();
void DeleteExistingSolid();
void PrintSolidsInMemory();
void CopyExistingSolid();
void MoveASolid();
void DebugASolid();
void SaveAllObjects();
std::string PickFile();
std::string PickNewFile();
void LoadASolidBox();


int main(int argc, char** argv)
{
	numberInputs.insert(std::pair<std::string, int>("one", 1));
	numberInputs.insert(std::pair<std::string, int>("two", 2));
	numberInputs.insert(std::pair<std::string, int>("three", 3));
	numberInputs.insert(std::pair<std::string, int>("four", 4));
	numberInputs.insert(std::pair<std::string, int>("five", 5));
	numberInputs.insert(std::pair<std::string, int>("six", 6));
	numberInputs.insert(std::pair<std::string, int>("seven", 7));
	numberInputs.insert(std::pair<std::string, int>("eight", 8));
	numberInputs.insert(std::pair<std::string, int>("nine", 9));
	numberInputs.insert(std::pair<std::string, int>("ten", 10));
	

	bool quit = false;
	while (!quit) {
		WelcomeAndOptions();
		std::string text = start_listening();
		if (text == "one")
		{
			CreateSolidBox();
		}
		else if (text == "two")
		{
			DeleteExistingSolid();
		}
		else if (text == "three")
		{
			PrintSolidsInMemory();
		}
		else if (text == "four")
		{
			CopyExistingSolid();
		}
		else if (text == "five")
		{
			MoveASolid();
		}
		else if (text == "six")
		{
			DebugASolid();
		}
		else if (text == "seven")
		{
			SaveAllObjects();
		}
		else if (text == "eight")
		{
			LoadASolidBox();
		}
		if (text == "quit") quit = true;
		//std::cout << "Word: " << text << std::endl;
		//Utility::PrintDebugInfo(SolidBox::cubeVec.begin());
	}
	return 0;
}


std::string start_listening()
{
	// Initialize COM library
	if (FAILED(::CoInitialize(nullptr))) {
		return "ERROR";
	}

	HRESULT hr;
	ISpRecognizer* recognizer;
	hr = CoCreateInstance(CLSID_SpSharedRecognizer,
		nullptr, CLSCTX_ALL, IID_ISpRecognizer,
		reinterpret_cast<void**>(&recognizer));

	ISpRecoContext* recoContext;
	hr = recognizer->CreateRecoContext(&recoContext);

	// Disable context
	hr = recoContext->Pause(0);

	ISpRecoGrammar* recoGrammar = init_grammar(recoContext);

	hr = recoContext->SetNotifyWin32Event();

	HANDLE handleEvent;
	handleEvent = recoContext->GetNotifyEventHandle();

	ULONGLONG interest;
	interest = SPFEI(SPEI_RECOGNITION);
	hr = recoContext->SetInterest(interest, interest);

	// Activate Grammar
	hr = recoGrammar->SetRuleState(ruleName1, 0, SPRS_ACTIVE);

	// Enable context
	hr = recoContext->Resume(0);
	
	// Wait for reco
	HANDLE handles[1];
	handles[0] = handleEvent;
	WaitForMultipleObjects(1, handles, FALSE, INFINITE);
	std::string  text = get_text(recoContext);

	recoGrammar->Release();
	::CoUninitialize();
	return text;
}

/**
* Create and initialize the Grammar.
* Create a rule for the grammar.
* Add word to the grammar.
*/
ISpRecoGrammar* init_grammar(ISpRecoContext* recoContext)
{
	HRESULT hr;
	SPSTATEHANDLE sate;

	ISpRecoGrammar* recoGrammar;
	hr = recoContext->CreateGrammar(grammarId, &recoGrammar);
	WORD langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	recoGrammar->ResetGrammar(langId);
	recoGrammar->GetRule(ruleName1, 0, SPRAF_TopLevel | SPRAF_Active, true, &sate);
	std::wstring words[] = { L"one", L"two", L"three", L"four", L"five", L"six", L"seven", L"eight", L"nine", L"ten" , L"quit"};
	for (auto &w : words) {
		recoGrammar->AddWordTransition(sate, NULL, w.c_str(), L" ", SPWT_LEXICAL, 1, nullptr);
	}

	recoGrammar->Commit(0);

	/*
	recoGrammar->LoadDictation(NULL, SPLO_STATIC);
	recoGrammar->SetDictationState(SPRS_ACTIVE);
	*/
	return recoGrammar;
}
std::string ToNarrow(const wchar_t *s, char dfault = '?',
	const std::locale& loc = std::locale())
{
	std::ostringstream stm;

	while (*s != L'\0') {
		stm << std::use_facet< std::ctype<wchar_t> >(loc).narrow(*s++, dfault);
	}
	return stm.str();
}

std::string get_text(ISpRecoContext* reco_context)
{
	const ULONG maxEvents = 10;
	SPEVENT events[maxEvents];

	ULONG eventCount;
	HRESULT hr;
	hr = reco_context->GetEvents(maxEvents, events, &eventCount);

	ISpRecoResult* recoResult;
	recoResult = reinterpret_cast<ISpRecoResult*>(events[0].lParam);

	wchar_t* text;
	hr = recoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, FALSE, &text, NULL);
	std::string test = ToNarrow(text);

	CoTaskMemFree(text);
	return test;
}

void WelcomeAndOptions()
{
	std::cout << "What would you like to do?" << std::endl;
	std::cout << "(Say a number or say 'quit' to quit)\n\n" << std::endl;
	std::cout << "1)  Create a solid box" << std::endl;
	std::cout << "2)  Delete a solid box" << std::endl;
	std::cout << "3)  Show available solids" << std::endl;
	std::cout << "4)  Copy an existing solid into a new one" << std::endl;
	std::cout << "5)  Move an existing solid's data into another existing solid" << std::endl;
	std::cout << "6)  Debug a solid (print information)" << std::endl;
	std::cout << "7)  Save" << std::endl;
	std::cout << "8)  Load" << std::endl;
}

void CreateSolidBox()
{
	std::string text = "";
	int nSideLength = 0;
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "What would you like the length, width, and height to be? (in cm)" << std::endl;
	std::cout << "ex: 4" << std::endl;
	text = start_listening();
	while (numberInputs.count(text) == 0)
	{
		text = start_listening();
	}
	auto itr = numberInputs.find(text);
	nSideLength = (itr->second); // converting string input into an int

	std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(nSideLength);
	SolidBox::cubeVec.push_back(box);
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void DeleteExistingSolid()
{
	std::string strInput = "";
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Say the number corresponding to the cube you wish to delete" << std::endl;
	Utility::PrintSolidsInMemory();
	std::cout << std::endl;
	strInput = start_listening();
	while (numberInputs.count(strInput) == 0)
	{
		strInput = start_listening();
	}
	auto itr = numberInputs.find(strInput);
	if (itr->second < 0 || itr->second > SolidBox::cubeVec.size())
	{
		std::cout << "out of bounds.  Please try again." << std::endl;
		DeleteExistingSolid();
	}
	auto cubeVecItr = SolidBox::cubeVec.begin() + (itr->second - 1); // advance iterator by the # given by user 
	(**cubeVecItr).Delete(); // dereference twice (first for iterator, second for shared ptr)
}

void PrintSolidsInMemory()
{
	int count = 1;
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
		std::cout << count << ")  " << cubePtr->name << " (" << std::fixed << std::setprecision(3) << cubePtr->sideLength << ")" << std::endl;
		++count;
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
	Utility::PrintSolidsInMemory();
	std::cout << std::endl;
	std::cout << "Enter the number corresponding to the cube you wish to copy." << std::endl;
	std::string strInput = start_listening();

	while (numberInputs.count(strInput) == 0)
	{
		strInput = start_listening();
	}

	auto itr = numberInputs.find(strInput);

	if (itr->second < 0 || itr->second > SolidBox::cubeVec.size())
	{
		std::cout << "out of bounds.  Please try again." << std::endl;
		CopyExistingSolid();
	}

	auto cubeVecItr = SolidBox::cubeVec.begin();
	cubeVecItr = std::next(cubeVecItr, (itr->second - 1)); // advance iterator by the # given by user 
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

	PrintSolidsInMemory();
	std::cout << std::endl;
	std::cout << "Please say the number corresponding to the solid box you are moving from" << std::endl;
	strMoveFrom = start_listening();

	while (numberInputs.count(strMoveFrom) == 0)
	{
		strMoveFrom = start_listening();
	}

	std::cout << "Please say the number corresponding to the solid box you are moving to" << std::endl;
	strMoveTo = start_listening();

	while (numberInputs.count(strMoveTo) == 0)
	{
		strMoveTo = start_listening();
	}
	
	auto cubeVecItr_From = numberInputs.find(strMoveFrom);

	if (cubeVecItr_From->second < 0 || cubeVecItr_From->second > SolidBox::cubeVec.size())
	{
		std::cout << "selection out of bounds.  Please start over" << std::endl;
		MoveASolid();
	}

	auto itr_From = std::next(SolidBox::cubeVec.begin(), (cubeVecItr_From->second - 1)); // find if name given is the name of a cube made 
	auto cubeVecItr_To = numberInputs.find(strMoveTo);;

	if (cubeVecItr_To->second < 0 || cubeVecItr_To->second > SolidBox::cubeVec.size())
	{
		std::cout << "selection out of bounds.  Please start over" << std::endl;
		MoveASolid();
	}

	auto itr_To = std::next(SolidBox::cubeVec.begin(), (cubeVecItr_To->second - 1)); // find if name given is the name of a cube made 
	//check if trying to move to the same cube
	if (strMoveFrom == strMoveTo)
	{
		std::cout << "You cannot move from and to the same cube.  Please try again." << std::endl;
		MoveASolid();
	}

	// It's OK to now move From into To
	**itr_To = **itr_From;
}

void DebugASolid()
{
	std::string strInput = "";
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	std::cout << "Say the number corresponding to the desired cube for detailed information" << std::endl;
	PrintSolidsInMemory();
	std::cout << std::endl;
	strInput = start_listening();

	while (numberInputs.count(strInput) == 0)
	{
		strInput = start_listening();
	}

	auto itr = numberInputs.find(strInput);

	if (itr->second < 0 || itr->second > SolidBox::cubeVec.size())
	{
		std::cout << "selection out of bounds.  Please start over" << std::endl;
		MoveASolid();
	}

	auto cubeVecItr = SolidBox::cubeVec.begin() + (itr->second - 1); // advance iterator by the # given by user 
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
	Utility::PrintDebugInfo(cubeVecItr);
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

void SaveAllObjects()
{
	int count = 0;
	std::string strInput = "";
	std::string fileName = "";
	//if no boxes to save, return and don't save
	if (!Utility::IsOkToSave())
	{
		std::cout << "There are no solid boxes to save." << std::endl;
		return;
	}
	// view current files in memory and get input from the user to go to main menu,
	// save a new file, or replace an existing file
	Utility::ViewFiles();
	
//	fileName = Utility::PickNewFile();
	
	// saving objects to file now
	CFile solidBoxFile;
	solidBoxFile.Open("box.txt", CFile::modeCreate | CFile::modeWrite);
	CArchive archive(&solidBoxFile, CArchive::store);
	archive << SolidBox::nameIDCounter << ConnectionChannel::nameIDCounter << SquarePlane::nameIDCounter;
	archive << static_cast<int>(SolidBox::cubeVec.size());
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

		Utility::SaveASolidBox(cubePtr, archive, solidBoxFile); // does the actual saving of the objects
	}

	archive.Close();
	solidBoxFile.Close();
	Utility::PrintNwLnsAndLnDelimiter("-", 55);
}

std::string PickFile()
{
	std::string strInput = "";
	int numOfFiles = 0;
	int fileSelection = 0;
	WIN32_FIND_DATA file;
	HANDLE searchHandle = FindFirstFile("*.txt", &file);

	std::cout << "Please say a number that corresponds to the file" << std::endl;
	Utility::ViewFiles();
	
	strInput = start_listening();

	while (numberInputs.count(strInput) == 0)
	{
		strInput = start_listening();
	}

	auto itr = numberInputs.find(strInput);
	numOfFiles = Utility::NumOfFilesAvail();
	if (itr->second < 1 || itr->second > numOfFiles)
	{
		std::cout << "selection out of bounds.  Please start over" << std::endl;
		PickFile();
	}

	auto cubeVecItr = SolidBox::cubeVec.begin() + (itr->second - 1); // advance iterator by the # given by user 
	fileSelection = (itr->second);

	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		for (int ii = 1; ii < fileSelection; ++ii) // starting at 1 so you don't go to next file with 
		{                                          // FindNextFile if there's only one file or make rest
			FindNextFile(searchHandle, &file);     // of the files off by one (even if more than 1 file)
		}
	}
	else
	{
		std::cout << "A problem has occurred.  No such file in memory" << std::endl;
		return (strInput = "");
	}

	std::string fileName(file.cFileName);
	FindClose(searchHandle);
	return fileName;
}

std::string PickNewFile()
{
	std::string fileName = "";
	std::cout << "Please say the name (no extension) of the new file" << std::endl;
	std::cout << "For example, \"myFile\" would be acceptable (no quotes)" << std::endl;
	fileName = start_listening();
	fileName += ".txt";
	if (Utility::FileExists(fileName))
	{
		std::cout << fileName << " already exists.  Please make another selection." << std::endl;
		PickNewFile();
	}
	return fileName;
}

void LoadASolidBox()
{
	int numOfEdges = 0;
	double height = 0;
	double length = 0;
	char letter = 0;
	int vecSize = 0;
	int nameSize = 0;
	int solidBoxNameIDCntr = 0;
	int connChannelNmIDCntr = 0;
	int sqPlnNmIDCntr = 0;
	std::string name = "";
	bool hasConnection = false;
	std::string strInput = "";
	char cInput = 0;

	Utility::ViewFiles();
	std::cout << "Please say the number corresponding to the file you with to load" << std::endl;
	strInput = PickFile();

	CFile file;
	file.Open(strInput.c_str(), CFile::modeRead | CFile::typeBinary);
	CArchive ar(&file, CArchive::load);
	ar >> solidBoxNameIDCntr >> connChannelNmIDCntr >> sqPlnNmIDCntr;
	ar >> vecSize;

	//getting members for solidbox(es)
	for (int ii = 0; ii < vecSize; ++ii)
	{
		ar >> length >> hasConnection >> nameSize;
		name = ""; // resetting name for boxes if there is more than one solid box

		//getting cube name
		for (int ii = 0; ii < nameSize; ++ii)
		{
			ar >> letter;
			name += letter;
		}

		//constructing a solid box with given length and setting other params
		std::shared_ptr<SolidBox> box = std::make_shared<SolidBox>(length);
		box->SetName(name);
		ar >> nameSize;
		name = ""; // resetting name

		//getting connectionchannel name
		for (int ii = 0; ii < nameSize; ++ii)
		{
			ar >> letter;
			name += letter;
		}

		box->GetConnChannel()->SetName(name);

		//getting and setting members for square planes
		for (auto planePtr : box->GetConnChannel()->GetPlaneSet())
		{
			ar >> height >> length >> numOfEdges;
			planePtr->SetHeight(height);
			planePtr->SetLength(length);
			planePtr->SetNumOfEdges(numOfEdges);
			ar >> nameSize;
			name = ""; // resetting name

			for (int ii = 0; ii < nameSize; ++ii) // reading in sq plane name
			{
				ar >> letter;
				name += letter;
			}

			planePtr->SetName(name);
		}

		SolidBox::cubeVec.push_back(box); // solid box object is completed now.
	}

	// ensures counters are the same as when they were saved
	SolidBox::nameIDCounter = solidBoxNameIDCntr;
	ConnectionChannel::nameIDCounter = connChannelNmIDCntr;
	SquarePlane::nameIDCounter = sqPlnNmIDCntr;
	ar.Close();
	file.Close();
}