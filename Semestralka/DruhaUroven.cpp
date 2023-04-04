#include "DruhaUroven.h"

DruhaUroven::DruhaUroven()
{
    Citac citacKraje("data/kraje.csv");
    citacKraje.preskocPrvyRiadok();

    auto& sk = hierarchy.emplaceRoot();
    sk.data_.officialTitle = "Slovensko";

    size_t counter = 0;
    while (citacKraje.citajRiadok()) {
        UzemnaJednotka uj = citacKraje.vytvorUJ();
        auto& curSon = hierarchy.emplaceSon(sk, counter);

        curSon.data_.sortNumber = uj.sortNumber;
        curSon.data_.code = uj.code;
        curSon.data_.officialTitle = uj.officialTitle;
        curSon.data_.mediumTitle = uj.mediumTitle;
        curSon.data_.shortTitle = uj.shortTitle;
        curSon.data_.note = uj.note;
        curSon.data_.level = 1;
        ++counter;
    }

    citacKraje.zatvorSubor();

    Citac citacOkresy("data/okresy.csv");
    citacOkresy.preskocPrvyRiadok();
    int zahrCounter = 0;

    while (citacOkresy.citajRiadok()) {
        UzemnaJednotka uj = citacOkresy.vytvorUJ();
        if (uj.note.length() <= 1) {
            auto& currentKraj = *sk.sons_->access(8)->data_;
            auto& curSon = hierarchy.emplaceSon(currentKraj, zahrCounter);

            curSon.data_.sortNumber = uj.sortNumber;
            curSon.data_.code = uj.code;
            curSon.data_.officialTitle = uj.officialTitle;
            curSon.data_.mediumTitle = uj.mediumTitle;
            curSon.data_.shortTitle = uj.shortTitle;
            curSon.data_.note = uj.note;
            curSon.data_.level = 2;
            ++zahrCounter;
            continue;
        }
        int krajIndex = std::stoi(uj.note.substr(0, 1)) - 1;
        int sonIndex = std::stoi(uj.note.substr(uj.note.length() - 2, 2)) - 1;
        auto& currentKraj = *sk.sons_->access(krajIndex)->data_;
        auto& curSon = hierarchy.emplaceSon(currentKraj, sonIndex);

        curSon.data_.sortNumber = uj.sortNumber;
        curSon.data_.code = uj.code;
        curSon.data_.officialTitle = uj.officialTitle;
        curSon.data_.mediumTitle = uj.mediumTitle;
        curSon.data_.shortTitle = uj.shortTitle;
        curSon.data_.note = uj.note;
        curSon.data_.level = 2;
    }

    citacOkresy.zatvorSubor();

    Citac citacObce("data/obce.csv");
    citacObce.preskocPrvyRiadok();

    while (citacObce.citajRiadok()) {
        UzemnaJednotka uj = citacObce.vytvorUJ();
        int susEndIndex = uj.code.find(uj.note);
        std::string cleanCode = uj.code.substr(3, uj.code.length() - susEndIndex - 4);

        int krajIndex = 8;
        if (uj.code.find("ZZZ") != std::string::npos) {
            std::cout << "BDSFJHKDJHGJKDFGJDHKFGJHDKJGF\n";
            continue;
        }
        int cleanCodeInt = std::stoi(cleanCode);
        switch (cleanCodeInt) {
            case 10:
                krajIndex = 0;
                break;
            case 21 :
                krajIndex = 1;
                break;
            case 22:
                krajIndex = 2;
                break;
            case 23:
                krajIndex = 3;
                break;
            case 31:
                krajIndex = 4;
                break;
            case 32:
                krajIndex = 5;
                break;
            case 41:
                krajIndex = 6;
                break;
            case 42:
                krajIndex = 7;
                break;
        }

        int okresIndex = std::stoi(uj.code.substr(5, 1), nullptr, 16) - 1;
        auto& currentOkres = *sk.sons_->access(krajIndex)->data_->sons_->access(okresIndex)->data_;
        int fuckingIndex = currentOkres.sons_->size() - 1;
        auto& curSon = hierarchy.emplaceSon(currentOkres, fuckingIndex <= 0 ? 0 : fuckingIndex);

        curSon.data_.sortNumber = uj.sortNumber;
        curSon.data_.code = uj.code;
        curSon.data_.officialTitle = uj.officialTitle;
        curSon.data_.mediumTitle = uj.mediumTitle;
        curSon.data_.shortTitle = uj.shortTitle;
        curSon.data_.note = uj.note;
        curSon.data_.level = 3;
    }

    citacObce.zatvorSubor();

    for (auto it = hierarchy.beginPre(); it != hierarchy.endPre(); ++it) {
        std::cout << (*it).toStr() << std::endl;
    }
}
