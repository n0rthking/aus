#pragma once

#include "DatovaUroven.h"
#include <iostream>
#include <string>
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include "Algorithm.h"
#include "UzemnaJednotka.h"
#include "Citac.h"

using DataType = Obec;
using InputType = ds::amt::ImplicitSequence<DataType>;
using ResultType = ds::amt::ImplicitSequence<DataType>;

const int OPT_CONTAINS = 1;
const int OPT_STARTSWITH = 2;
const int OPT_INVALID = -1;

class PrvaUroven : public DatovaUroven
{
private:
	InputType inputSequence;
	ResultType outputSequence;
	std::string inputFilename;
	std::string userInput;
public:
	PrvaUroven();
	int zistiParametre();
	bool nacitajVstup();
	void filtrujZaznamyContains(const std::string& subString);
	void filtrujZaznamyStartsWith(const std::string& subString);
	void vypisVystup();
};

