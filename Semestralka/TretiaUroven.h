#pragma once

#include "DatovaUroven.h"
#include "DataTabulky.h"
#include <libds/adt/table.h>

class TretiaUroven : public DatovaUroven
{
private:
	//using DataType = Kraj*;
	using TableType = ds::adt::SortedSequenceTable<std::string, DataTabulky<Obec>*>;
	TableType tabulka;
public:
	TretiaUroven(const DatovaUroven& other);
};

