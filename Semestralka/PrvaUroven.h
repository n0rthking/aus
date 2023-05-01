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
	std::string unitType;
	std::string subString;
public:
    PrvaUroven(const DatovaUroven& other) : DatovaUroven(other)
    {
        int filteringOption = zistiParametre();
        if (filteringOption != OPT_INVALID) {
            if (this->unitType == "kraj") {
                SequenceType<Kraj*> outputSequence;
                filtrujZaznamy(this->seqKraje_, filteringOption, outputSequence);
                vypisVystup(outputSequence);
            }
            else if (this->unitType == "okres") {
                SequenceType<Okres*> outputSequence;
                filtrujZaznamy(this->seqOkresy_, filteringOption, outputSequence);
                vypisVystup(outputSequence);
            }
            else if (this->unitType == "obec") {
                SequenceType<Obec*> outputSequence;
                filtrujZaznamy(this->seqObce_, filteringOption, outputSequence);
                vypisVystup(outputSequence);
            }
        }
    }

    int zistiParametre()
    {
        std::cout << "Zadaj substring: ";
        std::getline(std::cin, this->subString);

        std::cout << "Zadaj typ [kraj, okres, obec]: ";
        std::cin >> this->unitType;

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

    template<typename DataType>
    void filtrujZaznamy(SequenceType<DataType>* inputSequence, int filteringOption, SequenceType<DataType*>& outputSequence)
    {
        std::function<bool(DataType)> lambdaContains = [&](const DataType& uj) { return uj.officialTitle.find(this->subString) != std::string::npos; };
        std::function<bool(DataType)> lambdaStartsWith = [&](const DataType& uj) { return uj.officialTitle.find(this->subString) == 0; };
        std::function<bool(DataType)> aktualnaLambda;

        if (filteringOption == OPT_CONTAINS) {
            aktualnaLambda = lambdaContains;
        }
        if (filteringOption == OPT_STARTSWITH) {
            aktualnaLambda = lambdaStartsWith;
        }

        Algorithm::findElementsWithProperty(
            inputSequence->begin(),
            inputSequence->end(),
            aktualnaLambda,
            outputSequence,
            [](SequenceType<DataType*>& result, DataType& data) {
                result.insertLast().data_ = &data;
            });
    }

    template<typename DataType>
    void vypisVystup(SequenceType<DataType>& inputSequence)
    {
        std::cout << "Filtrovane zaznamy:" << std::endl << std::endl;
        size_t pocet = 0;
        for (const auto& element : inputSequence) {
            std::cout << element->officialTitle << std::endl;
            ++pocet;
        }
        std::cout << "\nPocet filtrovanych zaznamov: " << pocet << "\n";
    }
};

