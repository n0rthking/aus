#include "PrvaUroven.h"

PrvaUroven::PrvaUroven()
{
    int retVal = zistiParametre();
    if (retVal != OPT_INVALID) {
        if (nacitajVstup()) {
            if (retVal == OPT_CONTAINS) {
                filtrujZaznamyContains(userInput);
            }
            if (retVal == OPT_STARTSWITH) {
                filtrujZaznamyStartsWith(userInput);
            }
            vypisVystup();
        }
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

bool PrvaUroven::nacitajVstup()
{
    Citac citac("data/" + inputFilename);

    if (!citac.skontrolujSubor()) {
        std::cout << "Chybne zadany subor" << std::endl;
        return false;
    }

    size_t pocet = 0;
    citac.preskocPrvyRiadok();

    while (citac.citajRiadok()) {
        UzemnaJednotka uj = citac.vytvorUJ();
        inputSequence.insertLast().data_ = uj;
        ++pocet;
    }

    std::cout << "Nacitanych " << pocet << " zaznamov zo suboru " << inputFilename << std::endl;
    citac.zatvorSubor();

    return true;
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
