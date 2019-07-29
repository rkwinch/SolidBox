#define NOMINMAX
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
#include <limits>
#include "Speech.h"
#include "Utility.h"

std::string Speech::StartListeningPhrase()
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

	ISpRecoGrammar* recoGrammar = InitGrammarPhrase(recoContext);

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
	std::string  text = GetText(recoContext);

	recoGrammar->Release();
	::CoUninitialize();
	return text;
}

double Speech::RetrieveDouble()
{
	double dNum = 0.0;
	std::string input = StartListeningPhrase();
	return (dNum = ConvertPhraseToDouble(input));
}

int Speech::RetrievePosInteger()
{
	int nNum = 0;
	std::string input = "";

	do
	{
		input = StartListeningPhrase();
		std::cout << "input:  " << input << std::endl;
		if (input == "back") return -1;

		if (input == "quit") return -1;

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

ISpRecoGrammar* Speech::InitGrammarPhrase(ISpRecoContext* recoContext)
{
	HRESULT hr;
	SPSTATEHANDLE state;

	ISpRecoGrammar* recoGrammar;
	hr = recoContext->CreateGrammar(grammarId, &recoGrammar);
	WORD langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	recoGrammar->ResetGrammar(langId);
	recoGrammar->GetRule(ruleName1, 0, SPRAF_TopLevel | SPRAF_Active, true, &state);
	recoGrammar->Commit(0);
	recoGrammar->LoadDictation(NULL, SPLO_STATIC);
	recoGrammar->SetDictationState(SPRS_ACTIVE);
	return recoGrammar;
}

int Speech::CalculateEditDistance(std::string word1, std::string word2) 
{
	std::vector<std::vector<int>> dp(word1.length() + 1);
	for (auto &v : dp) v.resize(word2.length() + 1);

	for (size_t i = 0; i <= word1.length(); i++) 
	{
		for (size_t j = 0; j <= word2.length(); j++) 
		{
			if (!i) dp[i][j] = j;
			else if (!j) dp[i][j] = i;
			else if (word1[i - 1] == word2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
			else dp[i][j] = 1 + (std::min)({ dp[i][j - 1], dp[i - 1][j], dp[i - 1][j - 1] });
		}
	}

	return dp[word1.length()][word2.length()];
}

std::string Speech::ClampWord(std::string const &word) 
{
	std::string bestMatch = word;
	int minEditDistance = std::numeric_limits<int>::max();
	static std::vector<std::string> dictionary{ "zero", "one", "two", "three", "four", "five", "six", "seven", "eight",
		"nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", 
		"eighteen", "nineteen", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety",
		"hundred", "thousand", "million", "point", "back", "quit", "and" };

	for (auto &candidate : dictionary) {
		auto editDist = CalculateEditDistance(word, candidate);
		if (editDist < minEditDistance) {
			bestMatch = candidate;
			minEditDistance = editDist;
		}
	}

	return bestMatch;
}

std::string Speech::ClampPhrase(std::string const &text) {
	std::string rv;
	std::istringstream is(text);
	std::string word;
	while (std::getline(is, word, ' ')) {
		rv += ClampWord(word) + " ";
	}
	return std::string(rv.begin(), rv.end() - 1);
}

int Speech::ConvertPhraseToInteger(std::string input)
{
	std::istringstream strStream;
	std::string word;
	int current = 0;
	int result = 0;
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

	if ((input.find("back") != std::string::npos) || (input.find("quit") != std::string::npos))
	{
		return -1; // user elected return to main menu
	}

	input = ClampPhrase(input);
	wordsToNumMap["and"] = std::make_pair(1, 0);
	
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

	strStream.str(input);

	while (std::getline(strStream, word, ' '))
	{
		int scale = 0;
		int increment = 0;
		std::tie(scale, increment) = wordsToNumMap[word];

		current = current * scale + increment;

		if (scale > 100)
		{
			result += current;
			current = 0;
		}
	}
	std::cout << result + current << std::endl;
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
	input = Speech::ClampPhrase(input);
	wordsToNumMap["and"] = std::make_pair(1, 0);

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

	strStream.str(input);

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

void Speech::FileNameConversion(std::string &fileName)
{   // removing special characters
	fileName.erase(std::remove_if(fileName.begin(), fileName.end(),
		[&](char element) {return (!isalpha(element) && (element != ' ') && (!isdigit(element))); }), fileName.end());
	
	// making letter camel case after making all of them lowercase (more uniform)
	for (int i = 0; i < static_cast<int>(fileName.size()); ++i)
	{
		fileName[i] = tolower(fileName[i]);

		if ((i > 0) && (fileName[i - 1] == ' ') && (!isdigit(fileName[i])))
		{
			fileName[i] = toupper(fileName[i]);
		}
	}

	// removing spaces and adding file extension of .txt
	fileName.erase(std::remove_if(fileName.begin(), fileName.end(), [&](char element) {return element == ' '; }), fileName.end());
	fileName += ".txt";
}