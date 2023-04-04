#pragma once

#include <iostream>
#include <string>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include "Algorithm.h"
#include "UzemnaJednotka.h"
#include "Citac.h"

using DataType = UzemnaJednotka;
using InputType = ds::amt::ImplicitSequence<DataType>;
using ResultType = ds::amt::MultiWayExplicitHierarchy<DataType>;

class DruhaUroven
{
private:
	InputType inputSequence;
	ResultType hierarchy;
public:
	DruhaUroven();
};

