#include <string>
#include <sapi.h>
#include <sphelper.h>
#include <locale>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include "Speech.h"
#include "Utility.h"


std::string Speech::StartListening()
{
	std::string sample = "";
	if (FAILED(::CoInitialize(nullptr)))
	{
		return "ERROR";
	}

	HRESULT hr;
	ISpRecognizer* recognizer;
	hr = CoCreateInstance(CLSID_SpSharedRecognizer, nullptr, CLSCTX_ALL, IID_ISpRecognizer, reinterpret_cast<void**>(&recognizer));

	/*
	* add stuff
	* add stuff
	* add stuff
	*/

	return sample;
}

ISpRecoGrammar* Speech::SetGrammar(ISpRecoContext* context)
{

	/*
	* add stuff
	* add stuff
	* add stuff
	*/

	ISpRecoGrammar * sample = nullptr;
	return sample;
}

Speech::Speech()
{
	InitializeMap();
	InitializeKeyWords();
	InitializeUnits();
	InitializeScales();
	InitializeTens();
}

Speech::~Speech()
{

}

double Speech::ConvertKeywordsToDouble(std::vector<std::string> vec)
{
	double retDouble = 0.0;
	std::string value = "";
	bool isBegin = true;
	int lastScale = 0;
	int currentScale = 0;
	std::string element = "";

	for (size_t i = 0; i < vec.size(); ++i)
	{
		element = vec[i];

		if (isBegin)
		{
			auto itr = find(unitsVec.begin(), unitsVec.begin() + 18, element); // is 1-19
			auto itr2 = find(tensVec.begin() + 18, tensVec.begin() + 26, element);
			std::vector<std::string>::iterator scalesVecItr = scaleVec.begin();

			if (itr != unitsVec.end())
			{
				value += std::to_string(strToIntMap[element]);

				if (vec.size() > i + 1)
				{
					scalesVecItr = find(scaleVec.begin(), scaleVec.end(), vec[i + 1]);

					if (scalesVecItr != scaleVec.end())
					{
						if (*itr == "hundred")
						{
							value += "00";
						}
						else if (*itr == "thousand")
						{
							value += "000";
						}
						else if (*itr == "million")
						{
							value += "000000";
						}
						else
						{
							throw std::exception("Invalid scale.");
						}
						++i;
					}
				}
			}
			else if (itr2 != tensVec.end()) // is 20-90
			{
				value += std::to_string(strToIntMap[element]);

				if (vec.size() > i + 1) // check if next value is present and if it is a single digit
				{
					itr = find(unitsVec.begin(), unitsVec.end(), vec[i + 1]);
					
					if (itr != unitsVec.end())
					{
						value[value.size() - 1] = (char)(strToIntMap[vec[i + 1]]); // make singles digit match (would be zero otherwise)
						++i;
						
						if (vec.size() > i + 1)
						{
							scalesVecItr = find(scaleVec.begin(), scaleVec.end(), vec[i + 1]);
							
							if (scalesVecItr != scaleVec.end())
							{
								if (*itr == "hundred")
								{
									value += "00";
								}
								else if (*itr == "thousand")
								{
									value += "000";
								}
								else if (*itr == "million")
								{
									value += "000000";
								}
								else
								{
									throw std::exception("Invalid scale.");
								}
								++i;
							}
						}
					}
				}
			}
			else if()
			isBegin = false;
		}

	}

	return retDouble;
}

std::vector<std::string> Speech::ParseKeywords(std::string input)
{
	int numKeywords = 0;
	int counter = 0;
	std::string intermedString = Utility::FilterNonChars(input); // replace all non characters with spaces
	std::vector<std::string> vec = Utility::TokenizeStringToVec(intermedString, ' '); // tokenize input for further processing
	std::vector<std::string> returnVec;

	// getting keywords from vector of input
	for (auto element : vec)
	{
		auto itr = find(keywordVec.begin(), keywordVec.end(), element);

		if (itr != keywordVec.end())
		{
			returnVec.push_back(element);
		}
	}

	return returnVec;
}

void Speech::InitializeMap()
{
	strToIntMap.insert(std::pair<std::string, int>("one", 1));
	strToIntMap.insert(std::pair<std::string, int>("two", 2));
	strToIntMap.insert(std::pair<std::string, int>("three", 3));
	strToIntMap.insert(std::pair<std::string, int>("four", 4));
	strToIntMap.insert(std::pair<std::string, int>("five", 5));
	strToIntMap.insert(std::pair<std::string, int>("six", 6));
	strToIntMap.insert(std::pair<std::string, int>("seven", 7));
	strToIntMap.insert(std::pair<std::string, int>("eight", 8));
	strToIntMap.insert(std::pair<std::string, int>("nine", 9));
	strToIntMap.insert(std::pair<std::string, int>("ten", 10));
	strToIntMap.insert(std::pair<std::string, int>("eleven", 11));
	strToIntMap.insert(std::pair<std::string, int>("twelve", 12));
	strToIntMap.insert(std::pair<std::string, int>("thirteen", 13));
	strToIntMap.insert(std::pair<std::string, int>("fourteen", 14));
	strToIntMap.insert(std::pair<std::string, int>("fifteen", 15));
	strToIntMap.insert(std::pair<std::string, int>("sixteen", 16));
	strToIntMap.insert(std::pair<std::string, int>("seventeen", 17));
	strToIntMap.insert(std::pair<std::string, int>("eighteen", 18));
	strToIntMap.insert(std::pair<std::string, int>("nineteen", 19));

	strToIntMap.insert(std::pair<std::string, int>("twenty", 20));
	strToIntMap.insert(std::pair<std::string, int>("thirty", 30));
	strToIntMap.insert(std::pair<std::string, int>("fourty", 40));
	strToIntMap.insert(std::pair<std::string, int>("fifty", 50));
	strToIntMap.insert(std::pair<std::string, int>("sixty", 60));
	strToIntMap.insert(std::pair<std::string, int>("seventy", 70));
	strToIntMap.insert(std::pair<std::string, int>("eighty", 80));
	strToIntMap.insert(std::pair<std::string, int>("ninety", 90));
}

void Speech::InitializeKeyWords()
{
	keywordVec.resize(31);

	keywordVec[0] = "one";
	keywordVec[1] = "two";
	keywordVec[2] = "three";
	keywordVec[3] = "four";
	keywordVec[4] = "five";
	keywordVec[5] = "six";
	keywordVec[6] = "seven";
	keywordVec[7] = "eight";
	keywordVec[8] = "nine";
	keywordVec[9] = "ten";
	keywordVec[10] = "eleven";
	keywordVec[11] = "twelve";
	keywordVec[12] = "thirteen";
	keywordVec[13] = "fourteen";
	keywordVec[14] = "fifteen";
	keywordVec[15] = "sixteen";
	keywordVec[16] = "seventeen";
	keywordVec[17] = "eighteen";
	keywordVec[18] = "nineteen";

	keywordVec[19] = "twenty";
	keywordVec[20] = "thirty";
	keywordVec[21] = "fourty";
	keywordVec[22] = "fifty";
	keywordVec[23] = "sixty";
	keywordVec[24] = "seventy";
	keywordVec[25] = "eighty";
	keywordVec[26] = "ninety";

	keywordVec[27] = "hundred";
	keywordVec[28] = "thousand";
	keywordVec[29] = "million";
	keywordVec[30] = "point";
}

void Speech::InitializeUnits()
{
	unitsVec.resize(19);

	unitsVec[0] = "one";
	unitsVec[1] = "two";
	unitsVec[2] = "three";
	unitsVec[3] = "four";
	unitsVec[4] = "five";
	unitsVec[5] = "six";
	unitsVec[6] = "seven";
	unitsVec[7] = "eight";
	unitsVec[8] = "nine";

	unitsVec[9] = "ten";
	unitsVec[10] = "eleven";
	unitsVec[11] = "twelve";
	unitsVec[12] = "thirteen";
	unitsVec[13] = "fourteen";
	unitsVec[14] = "fifteen";
	unitsVec[15] = "sixteen";
	unitsVec[16] = "seventeen";
	unitsVec[17] = "eighteen";
	unitsVec[18] = "nineteen";
}

void Speech::InitializeSingleDigits()
{
	singleDigitsVec.resize(9);

	singleDigitsVec[0] = "one";
	singleDigitsVec[1] = "two";
	singleDigitsVec[2] = "three";
	singleDigitsVec[3] = "four";
	singleDigitsVec[4] = "five";
	singleDigitsVec[5] = "six";
	singleDigitsVec[6] = "seven";
	singleDigitsVec[7] = "eight";
	singleDigitsVec[8] = "nine";
}

void Speech::InitializeOthers()
{
	othersVec[0] = "ten";
	othersVec[1] = "eleven";
	othersVec[2] = "twelve";
	othersVec[3] = "thirteen";
	othersVec[4] = "fourteen";
	othersVec[5] = "fifteen";
	othersVec[6] = "sixteen";
	othersVec[7] = "seventeen";
	othersVec[8] = "eighteen";
	othersVec[9] = "nineteen";
}
void Speech::InitializeTens()
{
	tensVec.resize(8);

	tensVec[0] = "twenty";
	tensVec[1] = "thirty";
	tensVec[2] = "fourty";
	tensVec[3] = "fifty";
	tensVec[4] = "sixty";
	tensVec[5] = "seventy";
	tensVec[6] = "eighty";
	tensVec[7] = "ninety";
}

void Speech::InitializeScales()
{
	scaleVec.resize(3);

	scaleVec[0] = "hundred";
	scaleVec[1] = "thousand";
	scaleVec[2] = "million";
}