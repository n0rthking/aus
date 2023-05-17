#pragma once

#include <libds/amt/implicit_sequence.h>
#include <libds/adt/sorts.h>
#include <locale>
#include <string>
#include <iostream>

template<typename DataType>
class StvrtaUroven
{
private:
	ds::adt::QuickSort<DataType> quickSort;
    std::locale skLocale;
	const std::string SAMOHLASKY = { 'a', 'e', 'i', 'o', 'u', 'y', 'á', 'é', 'í', 'ó', 'ú', 'ý', 'ä', 'ô' };

public:
	StvrtaUroven(ds::amt::ImplicitSequence<DataType>& sekvencia) {
        this->skLocale = std::locale("Slovak_Slovakia.1250");

        std::cout << "Vyber utriedenie [a]lphabetical sort, [v]owelsCount sort, [d]o not sort:\n";
        std::string utried;
        std::cin >> utried;
        std::cin.ignore(256, '\n');

        if (utried.find("a") == 0) {
            quickSort.sort(sekvencia, [&](DataType fst, DataType snd) { return this->skLocale(fst->officialTitle, snd->officialTitle); });
            std::cout << "Vystup zoradeny abecedne\n";
        }
        else if (utried.find("v") == 0) {
            quickSort.sort(sekvencia, [&](DataType fst, DataType snd) { return this->spocitajSamohlasky(fst->officialTitle) < this->spocitajSamohlasky(snd->officialTitle); });
            std::cout << "Vystup zoradeny podla poctu samohlasok\n";
        }
        else {
            std::cout << "Vystup nebude utriedeny\n";
        }

        for (auto itSeq = sekvencia.begin(); itSeq != sekvencia.end(); ++itSeq) {
            bool odsadenie = !(utried.find("v") == 0 || utried.find("a") == 0);
            std::cout << "\x1B[33m" << (*itSeq)->toString(odsadenie) << "\033[0m";
            if (utried.find("v") == 0) {
                std::cout << " (" << this->spocitajSamohlasky((*itSeq)->officialTitle) << ")";
            }
            std::cout << "\n";
        }

        std::cout << "Pocet vysledkov: " << sekvencia.size() << "\n";
	}

private:
	size_t spocitajSamohlasky(const std::string vstup) {
		size_t pocet = 0;
		for (size_t i = 0; i < vstup.length(); ++i) {
			if (this->SAMOHLASKY.find(std::tolower(vstup.at(i), this->skLocale)) != std::string::npos) {
				++pocet;
			}
		}
		return pocet;
	}
};

