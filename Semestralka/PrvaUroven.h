#pragma once

#include "DatovaUroven.h"
#include <libds/amt/implicit_sequence.h>
#include "Algorithm.h"
#include "UzemnaJednotka.h"

class PrvaUroven : public DatovaUroven
{
private:
	const int OPT_CONTAINS = 1;
	const int OPT_STARTSWITH = 2;
	const int OPT_INVALID = -1;
	SequenceType<UzemnaJednotka> outputSequence;
	std::string unitType;
	std::string subString;
public:
    PrvaUroven(const DatovaUroven& other) : DatovaUroven(other)
    {
        int retVal = zistiParametre();
        if (retVal != OPT_INVALID) {
            std::function<bool(UzemnaJednotka)> lambdaContains = [&](const auto& uj) { return uj.officialTitle.find(this->subString) != std::string::npos; };
            std::function<bool(UzemnaJednotka)> lambdaStartsWith = [&](const auto& uj) { return uj.officialTitle.find(this->subString) == 0; };
            std::function<bool(UzemnaJednotka)> aktualnaLambda;

            if (retVal == OPT_CONTAINS) {
                aktualnaLambda = lambdaContains;
            }
            if (retVal == OPT_STARTSWITH) {
                aktualnaLambda = lambdaStartsWith;
            }

            if (unitType == "kraj") {
                filtrujZaznamy<Kraj>(this->seqKraje_, aktualnaLambda);
            }
            else if (unitType == "okres") {
                filtrujZaznamy<Okres>(this->seqOkresy_, aktualnaLambda);
            }
            else if (unitType == "obec") {
                filtrujZaznamy<Obec>(this->seqObce_, aktualnaLambda);
            }

            vypisVystup();
        }
    }

    int zistiParametre()
    {
        std::cout << "Zadaj substring: ";
        std::getline(std::cin, subString);

        std::cout << "Zadaj typ [kraj, okres, obec]: ";
        std::cin >> unitType;

        std::cout << "Zadaj [c]ontains alebo [s]tartsWith: ";
        std::string userOption;
        std::cin >> userOption;

        if (userOption.length() > 0) {
            std::string firstStr = userOption.substr(0, 1);
            if (firstStr == "c") {
                return OPT_CONTAINS;
            }
            else if (firstStr == "s") {
                return OPT_STARTSWITH;
            }
        }

        std::cout << "Zle zadana moznost" << std::endl;
        return OPT_INVALID;
    }

    template<typename DataType, typename LambdaType>
    void filtrujZaznamy(SequenceType<DataType>* inputSequence, LambdaType filteringLambda)
    {
        Algorithm::findElementsWithProperty(
            inputSequence->begin(),
            inputSequence->end(),
            filteringLambda,
            outputSequence,
            [](SequenceType<UzemnaJednotka>& result, const DataType& data) {
                result.insertLast().data_ = data;
            });
    }

    void vypisVystup()
    {
        std::cout << "Filtrovane zaznamy:" << std::endl << std::endl;
        size_t pocet = 0;
        for (const auto& element : outputSequence) {
            std::cout << element.officialTitle << std::endl;
            ++pocet;
        }
        std::cout << "\nPocet filtrovanych zaznamov: " << pocet << "\n";
    }
};

