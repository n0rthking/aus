#pragma once

#include <string>

const int TYP_KRAJ = 1;
const int TYP_OKRES = 2;
const int TYP_OBEC = 3;

class UzemnaJednotka {
public:
    int sortNumber;
    std::string code;
    std::string officialTitle;
    std::string mediumTitle;
    std::string shortTitle;
    std::string note;
    int level;
    int typ;

    UzemnaJednotka() {
        sortNumber = 0;
        code = "";
        officialTitle = "";
        mediumTitle = "";
        shortTitle = "";
        note = "";
        level = 0;
        typ = 0;
    }

    bool operator==(const UzemnaJednotka& other) const {
        if (this->sortNumber != other.sortNumber
            || this->code != other.code
            || this->officialTitle != other.officialTitle
            || this->mediumTitle != other.mediumTitle
            || this->shortTitle != other.shortTitle
            || this->note != other.note) {
            return false;
        }
        return true;
    }

    UzemnaJednotka operator=(const UzemnaJednotka& other) {
        if (this != &other) {
            this->sortNumber = other.sortNumber;
            this->code = other.code;
            this->officialTitle = other.officialTitle;
            this->mediumTitle = other.mediumTitle;
            this->shortTitle = other.shortTitle;
            this->note = other.note;
            this->level = other.level;
            this->typ = other.typ;
        }
        return *this;
    }

    std::string toStr() const {
        std::string levelStr = "";
        if (level != 0) {
            levelStr = std::string(level, '\t');
        }
        return levelStr + this->officialTitle + " " + std::to_string(this->typ);
    }
};
