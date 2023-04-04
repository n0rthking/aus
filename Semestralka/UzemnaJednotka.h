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
    int level;

    UzemnaJednotka() {
        sortNumber = 0;
        code = "";
        officialTitle = "";
        mediumTitle = "";
        shortTitle = "";
        note = "";
        level = 0;
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

    void nastavAtributy(const UzemnaJednotka& other) {
        this->sortNumber = other.sortNumber;
        this->code = other.code;
        this->officialTitle = other.officialTitle;
        this->mediumTitle = other.mediumTitle;
        this->shortTitle = other.shortTitle;
        this->note = other.note;
        this->level = other.level;
    }

    std::string toStr() const {
        std::string levelStr = "";
        if (level != 0) {
            levelStr = std::string(level, '\t');
        }
        return levelStr + std::to_string(this->sortNumber) + " " + this->code + " " + this->officialTitle + " " + this->note;
    }
};
