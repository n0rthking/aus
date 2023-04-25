#pragma once

#include "DatovaUroven.h"
#include <iostream>
#include <string>
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include "Algorithm.h"
#include "UzemnaJednotka.h"
#include "Citac.h"

const int OPT_CONTAINS = 1;
const int OPT_STARTSWITH = 2;
const int OPT_INVALID = -1;

class PrvaUroven : public DatovaUroven
{
private:
	SequenceType<UzemnaJednotka> outputSequence;
	std::string whichFile;
	std::string userInput;
public:
	PrvaUroven();
	int zistiParametre();
	void filtrujZaznamy(const std::string& subString, int retVal);
	void vypisVystup();
};

