#include "PrvaUroven.h"

PrvaUroven::PrvaUroven()
{
    if (zistiParametre()) {
        nacitajVstup();
        filtrujZaznamy();
        vypisVystup();
    }
}

bool PrvaUroven::zistiParametre()
{
    std::cout << "Zadaj nazov suboru: ";
    std::cin >> inputFilename;

    std::cout << "Zadaj substring: ";
    std::cin >> userInput;

    return true;
}

void PrvaUroven::nacitajVstup()
{
    rapidcsv::Document uj("data/" + inputFilename, rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams(';'));
    std::vector<std::string> nazov = uj.GetColumn<std::string>("officialTitle");
    for (const auto& element : nazov) {
        inputSequence.insertLast().data_ = element;
    }
}

void PrvaUroven::filtrujZaznamy()
{
    std::string subString = userInput;
    algorithm.findElementsWithProperty(
        inputSequence.begin(),
        inputSequence.end(),
        [subString](const DataType& uj) {
            return uj.officialTitle.find(subString) != std::string::npos;
        },
        outputSequence,
            [](ResultType& result, const DataType& data) {
            result.insertLast().data_ = data;
        });
}

void PrvaUroven::vypisVystup()
{
    std::cout << "Filtrovane zaznamy:" << std::endl << std::endl;
    for (const auto& element : outputSequence) {
        std::cout << element.officialTitle << std::endl;
    }
}
