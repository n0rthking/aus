#pragma once

#include "UzemnaJednotka.h"

class Obec : public UzemnaJednotka {
public:
	Obec() : UzemnaJednotka() {
		this->typ = TYP_OBEC;
	}
};
