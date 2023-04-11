#pragma once

#include <string>

const int TYP_KRAJ = 1;
const int TYP_OKRES = 2;
const int TYP_OBEC = 3;

const int TO_STRING_NO_INDEX = -1;

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

    bool operator==(const UzemnaJednotka& other) {
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

    std::string toString(bool odsadenie = true, int index = TO_STRING_NO_INDEX) {
        std::string vystup = this->vratOdsadenie(odsadenie);
        if (index != TO_STRING_NO_INDEX) {
            return vystup + std::to_string(index) + ": \x1B[36m" + this->officialTitle + "\033[0m";
        }
        return vystup + this->officialTitle;
    }
private:
    std::string vratOdsadenie(bool odsadenie) {
        std::string odsadenieStr = "";

        if (!odsadenie) {
            return odsadenieStr;
        }
        
        switch (level) {
        case 1:
            odsadenieStr = "  ";
            break;
        case 2:
            odsadenieStr = "    ";
            break;
        case 3:
            odsadenieStr = "      ";
            break;
        case 4:
            odsadenieStr = "        ";
            break;
        }

        return odsadenieStr;
    }
};
