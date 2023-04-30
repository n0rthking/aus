#pragma once

#include "DatovaUroven.h"
#include "DataTabulky.h"
#include <libds/adt/table.h>

class TretiaUroven : public DatovaUroven
{
private:
	template <typename Type>
	using TableType = ds::adt::SortedSequenceTable<std::string, DataTabulky<Type>>;

	TableType<Kraj> tabulkaKraje;
	TableType<Okres> tabulkaOkresy;
	TableType<Obec> tabulkaObce;
public:
	TretiaUroven(const DatovaUroven& other) : DatovaUroven(other) {
		this->naplnTabulku(this->seqKraje_, this->tabulkaKraje);
		this->naplnTabulku(this->seqOkresy_, this->tabulkaOkresy);
		this->naplnTabulku(this->seqObce_, this->tabulkaObce);

		std::string kluc;
		std::cout << "Zadaj kluc: ";
		std::cin >> kluc;
		DataTabulky<Obec>* vysledok;
		bool ok = tabulkaObce.tryFind(kluc, vysledok);
		if (ok) {
			vysledok->vypis();
		}
	}

	template <typename DataType>
	void naplnTabulku(SequenceType<DataType>* inputSequence, TableType<DataType>& inputTable) {
		for (auto it = inputSequence->begin(); it != inputSequence->end(); ++it) {
			inputTable.insertWithCollisionHandling((*it).officialTitle, DataTabulky<DataType>(&(*it)));
		}
	}
};

