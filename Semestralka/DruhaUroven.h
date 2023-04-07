#pragma once

#include <iostream>
#include <string>
#include <libds/amt/explicit_hierarchy.h>
#include "Algorithm.h"
#include "UzemnaJednotka.h"
#include "Citac.h"

using DataType = UzemnaJednotka;
using ResultType = ds::amt::MultiWayExplicitHierarchy<DataType>;
using BlockResultType = ds::amt::MultiWayExplicitHierarchyBlock<DataType>;

const int OBLAST_ZAHRANICIE = 4;
const int KRAJ_ZAHRANICIE = 0;

class DruhaUroven
{
private:
	ResultType hierarchy;
public:
	DruhaUroven();
	void VytvorHierarchiu();
	void NacitajKraje();
	void NacitajOkresy();
	void NacitajObce();
private:
	BlockResultType& vratKraj(std::string identifikator, size_t zaciatok);
	BlockResultType& vratZahranicieKraj();
};

