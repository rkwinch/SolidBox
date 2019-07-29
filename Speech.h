#pragma once

#include <string>
#include <sapi.h>
#include <map>
#include <vector>


class Speech {

public:

	

	std::string StartListening();
	ISpRecoGrammar* SetGrammar(ISpRecoContext* context);
	Speech();
	~Speech();
	std::vector<std::string> ParseKeywords(std::string input);
	double ConvertKeywordsToDouble(std::vector<std::string> vec);
	
private:

	std::map<std::string, std::string> strToStrMap;
	std::map<std::string, int> strToIntMap;
	std::vector<std::string> keywordVec;
	std::vector<std::string> unitsVec;
	std::vector<std::string> tensVec;
	std::vector<std::string> scaleVec;
	std::vector<std::string> singleDigitsVec;
	std::vector<std::string> othersVec;
	
	void InitializeOthers();
	void InitializeSingleDigits();
	void InitializeScales();
	void InitializeTens();
	void InitializeUnits();
	void InitializeMap();
	void InitializeKeyWords();
	//std::vector<std::string> ParseKeywords(std::string input);

};