#pragma once

#include "DatovaUroven.h"
#include "DataTabulky.h"
#include <libds/adt/table.h>

class TretiaUroven : public DatovaUroven
{
private:
	template<typename Type>
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
		std::getline(std::cin, kluc);

		std::string zvolenyTyp;
		std::cout << "Zadaj typ: ";
		std::cin >> zvolenyTyp;

		if (zvolenyTyp.find("kraj") != std::string::npos) {
			this->vyhladajVypis<Kraj>(this->tabulkaKraje, kluc);
		}
		else if (zvolenyTyp.find("okres") != std::string::npos) {
			this->vyhladajVypis<Okres>(this->tabulkaOkresy, kluc);
		}
		else if (zvolenyTyp.find("obec") != std::string::npos) {
			this->vyhladajVypis<Obec>(this->tabulkaObce, kluc);
		}
		else {
			std::cout << "Zle zadany typ\n";
		}
	}

	template<typename DataType>
	void vyhladajVypis(TableType<DataType>& inputTable, const std::string& kluc) {
		DataTabulky<DataType>* vysledok;
		if (inputTable.tryFind(kluc, vysledok)) {
			vysledok->vypis();
		}
	}

	template<typename DataType>
	void naplnTabulku(SequenceType<DataType>* inputSequence, TableType<DataType>& inputTable) {
		for (auto it = inputSequence->begin(); it != inputSequence->end(); ++it) {
			inputTable.insertWithCollisionHandling((*it).officialTitle, DataTabulky<DataType>(&(*it)));
		}
	}
};

