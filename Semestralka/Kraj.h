#pragma once

#include "UzemnaJednotka.h"

class Kraj : public UzemnaJednotka {
public:
	Kraj() : UzemnaJednotka() {
		this->typ = TYP_KRAJ;
	}
};
