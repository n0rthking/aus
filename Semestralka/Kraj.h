#pragma once

#include <string>

class Kraj {
public:
    int sortNumber;
    std::string code;
    std::string officialTitle;
    std::string mediumTitle;
    std::string shortTitle;
    std::string note;

    Kraj() {
        sortNumber = 0;
        code = "";
        officialTitle = "";
        mediumTitle = "";
        shortTitle = "";
        note = "";
    }

    Kraj(const std::string& ot) {
        sortNumber = 0;
        code = "";
        officialTitle = ot;
        mediumTitle = "";
        shortTitle = "";
        note = "";
    }
};
