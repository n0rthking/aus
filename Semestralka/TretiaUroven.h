#pragma once

#include "DatovaUroven.h"
#include <libds/adt/table.h>

class TretiaUroven : public DatovaUroven
{
private:
	using DataType = UzemnaJednotka*;
	using TableType = ds::adt::SortedSequenceTable<std::string, DataType>;
public:
	TretiaUroven();
};

