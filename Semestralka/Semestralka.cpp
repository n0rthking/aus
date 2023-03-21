#include <iostream>
#include <string>
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include "rapidcsv.h"
#include "Algorithm.h"
#include "UzemnaJednotka.h"
#include <windows.h>

using DataType = UzemnaJednotka;
using ResultType = ds::amt::ImplicitSequence<DataType>;

int main() {
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    std::string subor;
    std::cout << "Zadaj nazov suboru: ";
    std::cin >> subor;

    rapidcsv::Document uj("data/" + subor, rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams(';'));

    std::vector<std::string> nazov = uj.GetColumn<std::string>("officialTitle");

    ResultType inputSeq;
    ResultType resultSeq;

    for (const auto& element : nazov) {
        inputSeq.insertLast().data_ = element;
    }

    Algorithm<DataType, ResultType> algo;
    std::string substr;
    std::cout << "Zadaj substring: ";
    std::cin >> substr;

    algo.findElementsWithProperty(
        inputSeq.begin(),
        inputSeq.end(),
        [substr](const DataType& obec) {
            return obec.officialTitle.find(substr) != std::string::npos;
        },
        resultSeq,
            [](ResultType& result, const DataType& data) {
            result.insertLast().data_ = data;
        });

    std::cout << "---------------" << std::endl;

    for (const auto& element : resultSeq) {
        std::cout << element.officialTitle << std::endl;
    }

    std::cout << "---------------" << std::endl;

    return 0;
}
