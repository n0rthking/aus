#pragma once

#include <libds/amt/explicit_hierarchy.h>
#include <libds/amt/implicit_sequence.h>
#include "DatovaUroven.h"
#include "Algorithm.h"
#include "OblastNazov.h"
#include "UzemnaJednotka.h"

class DruhaUroven : public DatovaUroven
{
private:
	const int OBLAST_ZAHRANICIE = 4;
	const int KRAJ_ZAHRANICIE = 0;
	using DataType = UzemnaJednotka*;
	using HierarchyType = ds::amt::MultiWayExplicitHierarchy<DataType>;
	using HierarchyBlockType = ds::amt::MultiWayExplicitHierarchyBlock<DataType>;
	HierarchyType hierarchy;
public:
	DruhaUroven(const DatovaUroven& other);
	~DruhaUroven();
	void VytvorHierarchiu();
	void RozhranieIteratora();
	void NacitajKraje();
	void NacitajOkresy();
	void NacitajObce();
private:
	HierarchyBlockType& vratKraj(std::string identifikator, size_t zaciatok);
	HierarchyBlockType& vratZahranicieKraj();
	void VypisAktualnuPoziciu(DataType uj);
	void VypisSynovNaAktualnejPozicii(ds::amt::IS<HierarchyBlockType*>* synovia);
	bool VstupOdUzivatela(ds::amt::Hierarchy<HierarchyBlockType>::PreOrderHierarchyIterator& it);
	void filtrujHierarchiu(ds::amt::Hierarchy<HierarchyBlockType>::PreOrderHierarchyIterator it);
};

