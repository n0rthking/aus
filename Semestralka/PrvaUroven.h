#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include "rapidcsv.h"
#include "Algorithm.h"
#include "UzemnaJednotka.h"

using DataType = UzemnaJednotka;
using InputType = ds::amt::ImplicitSequence<DataType>;
using ResultType = ds::amt::ImplicitSequence<DataType>;

class PrvaUroven
{
private:
	InputType inputSequence;
	ResultType outputSequence;
	Algorithm<DataType, ResultType> algorithm;
	std::string inputFilename;
	std::string userInput;
public:
	PrvaUroven();
	bool zistiParametre();
	void nacitajVstup();
	void filtrujZaznamy();
	void vypisVystup();
};

