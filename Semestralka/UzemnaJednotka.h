#pragma once

#include <string>

class UzemnaJednotka {
public:
    int sortNumber;
    std::string code;
    std::string officialTitle;
    std::string mediumTitle;
    std::string shortTitle;
    std::string note;

    UzemnaJednotka() {
        sortNumber = 0;
        code = "";
        officialTitle = "";
        mediumTitle = "";
        shortTitle = "";
        note = "";
    }

    UzemnaJednotka(const std::string& ot) {
        sortNumber = 0;
        code = "";
        officialTitle = ot;
        mediumTitle = "";
        shortTitle = "";
        note = "";
    }
};
