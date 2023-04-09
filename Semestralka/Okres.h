#pragma once

#include "UzemnaJednotka.h"

class Okres : public UzemnaJednotka {
public:
	Okres() : UzemnaJednotka() {
		this->typ = TYP_OKRES;
	}
};
