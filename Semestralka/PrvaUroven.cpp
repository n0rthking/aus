#include "PrvaUroven.h"

PrvaUroven::PrvaUroven()
{
    int retVal = zistiParametre();
    if (retVal != OPT_INVALID) {
        filtrujZaznamy(userInput, retVal);
        vypisVystup();
    }
}

int PrvaUroven::zistiParametre()
{
    std::cout << "Zadaj kraje, okresy alebo obce: ";
    std::cin >> whichFile;

    std::cout << "Zadaj substring: ";
    std::cin >> userInput;

    std::cout << "Zadaj [c]ontains alebo [s]tartsWith: ";
    std::string userOptStr;
    std::cin >> userOptStr;

    if (userOptStr.length() > 0) {
        std::string firstStr = userOptStr.substr(0, 1);
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

void PrvaUroven::filtrujZaznamy(const std::string& subString, int retVal)
{
    std::function<bool(UzemnaJednotka)> lambdaContains = [subString](const auto& uj) { return uj.officialTitle.find(subString) != std::string::npos; };
    std::function<bool(UzemnaJednotka)> lambdaStartsWith = [subString](const auto& uj) { return uj.officialTitle.find(subString) == 0; };
    std::function<bool(UzemnaJednotka)> aktualnaLambda;

    if (retVal == OPT_CONTAINS) {
        aktualnaLambda = lambdaContains;
    }
    if (retVal == OPT_STARTSWITH) {
        aktualnaLambda = lambdaStartsWith;
    }

    if (whichFile == "kraje") {
        SequenceType<Kraj>* inputSequence = &this->seqKraje_;
        Algorithm::findElementsWithProperty(
            inputSequence->begin(),
            inputSequence->end(),
            aktualnaLambda,
            outputSequence,
            [](auto& result, const auto& data) {
                result.insertLast().data_ = data;
            });
    }

    else if (whichFile == "okresy") {
        SequenceType<Okres>* inputSequence = &this->seqOkresy_;
        Algorithm::findElementsWithProperty(
            inputSequence->begin(),
            inputSequence->end(),
            aktualnaLambda,
            outputSequence,
            [](auto& result, const auto& data) {
                result.insertLast().data_ = data;
            });
    }

    else if (whichFile == "obce") {
        SequenceType<Obec>* inputSequence = &this->seqObce_;
        Algorithm::findElementsWithProperty(
            inputSequence->begin(),
            inputSequence->end(),
            aktualnaLambda,
            outputSequence,
            [](auto& result, const auto& data) {
                result.insertLast().data_ = data;
            });
    }
}

void PrvaUroven::vypisVystup()
{
    std::cout << "Filtrovane zaznamy:" << std::endl << std::endl;
    size_t pocet = 0;
    for (const auto& element : outputSequence) {
        std::cout << element.officialTitle << std::endl;
        ++pocet;
    }
    std::cout << "\nPocet filtrovanych zaznamov: " << pocet << "\n";
}
