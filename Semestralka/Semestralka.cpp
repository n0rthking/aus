#include <iostream>
#include <string>
#include <vector>
#include <libds/amt/implicit_sequence.h>
#include "rapidcsv.h"
#include "Algorithm.h"
#include "Obec.h"
#include "Kraj.h"
#include "Okres.h"
#include <windows.h>

using DataType = Obec;
using ResultType = ds::amt::ImplicitSequence<DataType>;

int main() {
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    rapidcsv::Document obce("data/obce.csv",
        rapidcsv::LabelParams(0, 0),
        rapidcsv::SeparatorParams(';'));

    std::vector<std::string> nazov = obce.GetColumn<std::string>("officialTitle");

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

    for (auto& element : resultSeq) {
        std::cout << element.officialTitle << std::endl;
    }

    std::cout << "---------------" << std::endl;

    return 0;
}
