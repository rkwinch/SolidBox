#include <string>
#include <sapi.h>
#include <sphelper.h>
#include <locale>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <regex>
#include "Speech.h"
#include "Utility.h"

std::string Speech::StartListening()
{
	if (FAILED(::CoInitialize(nullptr)))
	{
		return "ERROR";
	}

	HRESULT hr;
	ISpRecognizer* recognizer;
	ISpRecoContext* recoContext;
	ISpRecoGrammar* recoGrammar;
	HANDLE handleEvent;
	ULONGLONG interest;
	std::string text = "";

	hr = CoCreateInstance(CLSID_SpSharedRecognizer, nullptr, CLSCTX_ALL, IID_ISpRecognizer, reinterpret_cast<void**>(&recognizer));
	hr = recognizer->CreateRecoContext(&recoContext);
	hr = recoContext->Pause(0);
	recoGrammar = InitGrammar(recoContext);
	hr = recoContext->SetNotifyWin32Event();
	handleEvent = recoContext->GetNotifyEventHandle();
	interest = SPFEI(SPEI_RECOGNITION);
	hr = recoContext->SetInterest(interest, interest);
	hr = recoGrammar->SetRuleState(ruleName1, 0, SPRS_ACTIVE);
	hr = recoContext->Resume(0);

	HANDLE handles[1];
	handles[0] = handleEvent;
	WaitForMultipleObjects(1, handles, FALSE, INFINITE);
	text = GetText(recoContext);
	recoGrammar->Release();
	::CoUninitialize();
	return text;
}

double Speech::RetrieveDouble()
{
	double dNum = 0.0;
	std::string input = StartListening();
	return (dNum = ConvertPhraseToDouble(input));
}

int Speech::RetrievePosInteger()
{
	int nNum = 0;
	std::string input = "";

	do
	{
		input = StartListening();

		if (input == "back") return -1;

		nNum = ConvertPhraseToInteger(input);

		if (nNum == -1) return -1; // user elected to quit

	} while (nNum < 0);

	return nNum;
}

std::string Speech::GetText(ISpRecoContext* reco_context)
{
	const ULONG maxEvents = 10;
	SPEVENT events[maxEvents];
	ULONG eventCount;
	HRESULT hr;
	std::string test = "";
	ISpRecoResult* recoResult;
	wchar_t* text;

	hr = reco_context->GetEvents(maxEvents, events, &eventCount);
	recoResult = reinterpret_cast<ISpRecoResult*>(events[0].lParam);
	hr = recoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, FALSE, &text, NULL);
	test = ToNarrow(text);
	CoTaskMemFree(text);
	return test;
}

std::string Speech::ToNarrow(const wchar_t *s, char dfault,
	const std::locale& loc)
{
	std::ostringstream stm;

	while (*s != L'\0') {
		stm << std::use_facet< std::ctype<wchar_t> >(loc).narrow(*s++, dfault);
	}
	return stm.str();
}

ISpRecoGrammar* Speech::InitGrammar(ISpRecoContext* recoContext)
{
	HRESULT hr;
	SPSTATEHANDLE state;
	ISpRecoGrammar* recoGrammar;

	hr = recoContext->CreateGrammar(grammarId, &recoGrammar);
	WORD langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	recoGrammar->ResetGrammar(langId);
	recoGrammar->GetRule(ruleName1, 0, SPRAF_TopLevel | SPRAF_Active, true, &state);
	std::wstring words[] = { L"one", L"two", L"three", L"four", L"five", L"six", L"seven", L"eight", L"nine", L"ten" , L"quit" };

	for (auto &item : words) {
		recoGrammar->AddWordTransition(state, NULL, item.c_str(), L" ", SPWT_LEXICAL, 1, nullptr);
	}

	recoGrammar->Commit(0);
	return recoGrammar;
}

int Speech::ConvertPhraseToInteger(std::string input)
{
	std::istringstream strStream;
	std::string word;
	int current = 0;
	int result = 0;
	std::vector<std::string> strVec;
	std::vector<std::string> trimmedVec;
	std::string trimmedInput = "";
	std::map<std::string, std::pair<int, int>> wordsToNumMap;

	for (auto &character : input) // making sure all of the input is uniformly lowercase
	{
		tolower(character);
	}

	for (size_t i = 0; i < input.size(); ++i) // removing all other characters that aren't letters, such as
	{                                         // hyphens, commas, periods, etc
		if (!isalpha(input[i]))
		{
			input[i] = ' ';
		}
	}

	if (input.find("back") != std::string::npos)
	{
		return -1; // user elected return to main menu
	}

	strVec = Utility::TokenizeStringToVec(input, ' '); // tokenize into vec by a single whitespace

	for (auto element : strVec) // only keeping words of interest (in units, scales, or tens)
	{
		auto unitsItr = find(units.begin(), units.end(), element);
		auto scalesItr = find(scales.begin(), scales.end(), element);
		auto tensItr = find(tens.begin(), tens.end(), element);

		if ((unitsItr != units.end()) || (scalesItr != scales.end()) || (tensItr != tens.end()))
		{
			trimmedVec.push_back(element);
		}
	}

	if (strVec.size() == 0)
	{              //will use this as a return flag that input was invalid     
		return -2; // user perhaps mumbled or said something else
	}

	for (auto element : trimmedVec) // putting back into string form
	{
		trimmedInput += (element + ' ');
	}

	trimmedInput.erase(trimmedInput.begin() + trimmedInput.size() - 1); // removing last space

	for (size_t i = 0; i < units.size(); ++i)
	{
		wordsToNumMap[units[i]] = std::make_pair(1, i);
	}

	for (size_t i = 0; i < tens.size(); ++i)
	{
		wordsToNumMap[tens[i]] = std::make_pair(1, 10 * i);
	}

	for (size_t i = 0; i < scales.size(); ++i)
	{
		wordsToNumMap[scales[i]] = std::make_pair(pow(10, i ? i * 3 : 2), 0);
	}

	strStream.str(trimmedInput);

	while (std::getline(strStream, word, ' '))
	{
		double scale = 0;
		double increment = 0;
		std::tie(scale, increment) = wordsToNumMap[word];

		current = current * scale + increment;

		if (scale > 100)
		{
			result += current;
			current = 0;
		}
	}

	return result + current;
}

double Speech::ConvertPhraseToDouble(std::string input)
{
	std::istringstream strStream;
	std::string word;
	double current = 0;
	double result = 0;
	double deci = 0;
	int deciIndex = 0;
	std::vector<std::string> strVec;
	std::vector<std::string> trimmedVec;
	std::string trimmedInput = "";
	std::map<std::string, std::pair<int, int>> wordsToNumMap;

	for (auto &character : input) // making sure all of the input is uniformly lowercase
	{
		tolower(character);
	}

	for (size_t i = 0; i < input.size(); ++i) // removing all other characters that aren't letters, such as
	{                                         // nyphens, commas, periods, etc
		if (!isalpha(input[i]))
		{
			input[i] = ' ';
		}
	}

	strVec = Utility::TokenizeStringToVec(input, ' ');

	for (auto element : strVec) // only keeping words of interest (in units, scales, tens, or is "point")
	{
		auto unitsItr = find(units.begin(), units.end(), element);
		auto scalesItr = find(scales.begin(), scales.end(), element);
		auto tensItr = find(tens.begin(), tens.end(), element);

		if ((unitsItr != units.end()) || (scalesItr != scales.end()) || (tensItr != tens.end()) || element == "point")
		{
			trimmedVec.push_back(element);
		}
	}

	for (auto element : trimmedVec)
	{
		trimmedInput += (element + ' ');
	}

	trimmedInput.erase(trimmedInput.begin() + trimmedInput.size() - 1); // removing last space

	for (size_t i = 0; i < units.size(); ++i)
	{
		wordsToNumMap[units[i]] = std::make_pair(1, i);
	}

	for (size_t i = 0; i < tens.size(); ++i)
	{
		wordsToNumMap[tens[i]] = std::make_pair(1, 10 * i);
	}

	for (size_t i = 0; i < scales.size(); ++i)
	{
		wordsToNumMap[scales[i]] = std::make_pair(pow(10, i ? i * 3 : 2), 0);
	}

	strStream.str(trimmedInput);

	while (std::getline(strStream, word, ' '))
	{
		if (word == "point")
		{
			deciIndex++;
		}

		if (deciIndex && (word != "point"))
		{
			auto val = wordsToNumMap[word].second;
			if ((std::find(units.begin(), units.end(), word) == units.end()) || val < 0 || val > 9)
			{
				throw std::exception("Invalid number.");
			}

			deci += val / std::pow(10.0, deciIndex++);
		}
		else if (!deciIndex)
		{
			double scale = 0;
			double increment = 0;
			std::tie(scale, increment) = wordsToNumMap[word];
			current = current * scale + increment;
			if (scale > 100)
			{
				result += current;
				current = 0;
			}
		}
	}

	return result + current + deci;
}