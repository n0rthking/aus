#include "PrvaUroven.h"

PrvaUroven::PrvaUroven()
{
    int retVal = zistiParametre();
    if (retVal != OPT_INVALID) {
        nacitajVstup();
        if (retVal == OPT_CONTAINS) {
            filtrujZaznamyContains(userInput);
        }
        if (retVal == OPT_STARTSWITH) {
            filtrujZaznamyStartsWith(userInput);
        }
        vypisVystup();
    }
}

int PrvaUroven::zistiParametre()
{
    std::cout << "Zadaj nazov suboru: ";
    std::cin >> inputFilename;

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

void PrvaUroven::nacitajVstup()
{
    rapidcsv::Document uj("data/" + inputFilename, rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams(';'));
    std::vector<std::string> nazov = uj.GetColumn<std::string>("officialTitle");
    for (const auto& element : nazov) {
        inputSequence.insertLast().data_ = element;
    }
}

void PrvaUroven::filtrujZaznamyContains(const std::string& subString)
{
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

void PrvaUroven::filtrujZaznamyStartsWith(const std::string& subString)
{
    algorithm.findElementsWithProperty(
        inputSequence.begin(),
        inputSequence.end(),
        [subString](const DataType& uj) {
            return uj.officialTitle.find(subString) == 0;
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
