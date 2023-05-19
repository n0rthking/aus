#pragma once

#include "DatovaUroven.h"
#include "DataTabulky.h"
#include <libds/adt/table.h>

class TretiaUroven : public DatovaUroven
{
private:
	template<typename Type>
	using TableType = ds::adt::Treap<std::string, DataTabulky<Type>>;

	TableType<Kraj> tabulkaKraje;
	TableType<Okres> tabulkaOkresy;
	TableType<Obec> tabulkaObce;
public:
	TretiaUroven(const DatovaUroven& other) : DatovaUroven(other) {
		this->naplnTabulku(this->seqKraje_, this->tabulkaKraje);
		this->naplnTabulku(this->seqOkresy_, this->tabulkaOkresy);
		this->naplnTabulku(this->seqObce_, this->tabulkaObce);
	}

	void spustiUroven() {
		std::string kluc;
		std::cout << "Zadaj kluc: ";
		std::getline(std::cin, kluc);

		std::string zvolenyTyp;
		std::cout << "Zadaj typ [kraj, okres, obec]: ";
		std::cin >> zvolenyTyp;
		std::cin.ignore(256, '\n');

		if (zvolenyTyp == "kraj") {
			this->vyhladajVypis(this->tabulkaKraje, kluc);
		}
		else if (zvolenyTyp == "okres") {
			this->vyhladajVypis(this->tabulkaOkresy, kluc);
		}
		else if (zvolenyTyp == "obec") {
			this->vyhladajVypis(this->tabulkaObce, kluc);
		}
		else {
			std::cout << "\x1B[31mNespravny typ\033[0m\n";
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

