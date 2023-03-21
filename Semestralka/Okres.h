#pragma once

#include <string>

class Okres {
public:
    int sortNumber;
    std::string code;
    std::string officialTitle;
    std::string mediumTitle;
    std::string shortTitle;
    std::string note;

    Okres() {
        sortNumber = 0;
        code = "";
        officialTitle = "";
        mediumTitle = "";
        shortTitle = "";
        note = "";
    }

    Okres(const std::string& ot) {
        sortNumber = 0;
        code = "";
        officialTitle = ot;
        mediumTitle = "";
        shortTitle = "";
        note = "";
    }
};
