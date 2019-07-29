#pragma once

#include <string>
#include <sapi.h>
#include <map>
#include <vector>
#include <regex>


namespace Speech {

	static const ULONGLONG grammarId = 0;
	static wchar_t* ruleName1 = L"ruleName1";
	static std::vector<std::string> units = { "zero", "one", "two", "three", "four", "five", "six",
											"seven", "eight", "nine", "ten", "eleven", "twelve",
											 "thirteen", "fourteen", "fifteen", "sixteen",
											 "seventeen", "eighteen", "nineteen" };
	static std::vector<std::string> tens = { "", "", "twenty", "thirty", "forty", "fifty", "sixty",
										  "seventy", "eighty", "ninety" };
	static std::vector<std::string> scales = { "hundred", "thousand", "million" };
	static std::map<std::string, std::string> strToStrMap;
	static std::map<std::string, int> strToIntMap;
	static std::vector<std::string> keywordVec;

	double RetrieveDouble();
	int RetrievePosInteger();
	ISpRecoGrammar* InitGrammarPhrase(ISpRecoContext* recoContext);
	std::string ClampWord(std::string const &word);
	std::string ClampPhrase(std::string const &text);
	std::string GetText(ISpRecoContext* reco_context);
	std::string ToNarrow(const wchar_t *s, char dfault = '?',
		const std::locale& loc = std::locale());
	double ConvertPhraseToDouble(std::string input);
	int ConvertPhraseToInteger(std::string input);
	int CalculateEditDistance(std::string word1, std::string word2);
	std::string StartListeningPhrase();
	void FileNameConversion(std::string &fileName);
}