#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include "UzemnaJednotka.h"

class Citac
{
private:
	std::ifstream suborStream;
	std::string aktualnyRiadok = "";
public:
	Citac(std::string nazovSuboru) {
		suborStream.open(nazovSuboru);
	}

	bool skontrolujSubor();
	bool citajRiadok();
	UzemnaJednotka vytvorUJ();
	void zatvorSubor();
};

