#pragma once

#include <iostream>
#include <string>
#include <libds/amt/explicit_hierarchy.h>
#include <libds/amt/implicit_sequence.h>
#include "DatovaUroven.h"
#include "Algorithm.h"
#include "OblastNazov.h"
#include "UzemnaJednotka.h"
#include "Kraj.h"
#include "Okres.h"
#include "Obec.h"

using DataType = UzemnaJednotka*;
using ResultType = ds::amt::MultiWayExplicitHierarchy<DataType>;
using BlockResultType = ds::amt::MultiWayExplicitHierarchyBlock<DataType>;

const int OBLAST_ZAHRANICIE = 4;
const int KRAJ_ZAHRANICIE = 0;

class DruhaUroven : public DatovaUroven
{
private:
	ResultType hierarchy;
public:
	DruhaUroven();
	~DruhaUroven();
	void VytvorHierarchiu();
	void RozhranieIteratora();
	void NacitajKraje();
	void NacitajOkresy();
	void NacitajObce();
private:
	BlockResultType& vratKraj(std::string identifikator, size_t zaciatok);
	BlockResultType& vratZahranicieKraj();
	void VypisAktualnuPoziciu(DataType uj);
	void VypisSynovNaAktualnejPozicii(ds::amt::IS<BlockResultType*>* synovia);
	bool VstupOdUzivatela(ds::amt::Hierarchy<BlockResultType>::PreOrderHierarchyIterator& it);
	void filtrujHierarchiu(ds::amt::Hierarchy<BlockResultType>::PreOrderHierarchyIterator it);
};

