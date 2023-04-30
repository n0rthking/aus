#pragma once

#include "DatovaUroven.h"
#include <iostream>
#include <string>
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include "Algorithm.h"
#include "UzemnaJednotka.h"
#include "Citac.h"

class PrvaUroven : public DatovaUroven
{
private:
	const int OPT_CONTAINS = 1;
	const int OPT_STARTSWITH = 2;
	const int OPT_INVALID = -1;
	SequenceType<UzemnaJednotka> outputSequence;
	std::string whichFile;
	std::string userInput;
public:
	PrvaUroven(const DatovaUroven& other);
	int zistiParametre();
	void filtrujZaznamy(const std::string& subString, int retVal);
	void vypisVystup();
};

