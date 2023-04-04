#include "DruhaUroven.h"

DruhaUroven::DruhaUroven()
{
    hierarchy.emplaceRoot().data_.officialTitle = "Slovensko";

    VytvorHierarchiu();

    for (auto it = hierarchy.beginPre(); it != hierarchy.endPre(); ++it) {
        std::cout << (*it).toStr() << std::endl;
    }
}

void DruhaUroven::VytvorHierarchiu()
{
    NacitajKraje();
    NacitajOkresy();
    NacitajObce();
}

void DruhaUroven::NacitajKraje()
{
    Citac citacKraje("data/kraje.csv");
    citacKraje.preskocPrvyRiadok();

    size_t counter = 0;
    while (citacKraje.citajRiadok()) {
        UzemnaJednotka uj = citacKraje.vytvorUJ();
        auto& aktualnySyn = hierarchy.emplaceSon(*hierarchy.accessRoot(), counter);

        aktualnySyn.data_.nastavAtributy(uj);
        aktualnySyn.data_.level = 1;
        ++counter;
    }

    citacKraje.zatvorSubor();
}

void DruhaUroven::NacitajOkresy()
{
    Citac citacOkresy("data/okresy.csv");
    citacOkresy.preskocPrvyRiadok();

    int zahrCounter = 0;
    while (citacOkresy.citajRiadok()) {
        UzemnaJednotka uj = citacOkresy.vytvorUJ();
        if (uj.note.length() <= 1) {
            auto& currentKraj = *hierarchy.accessRoot()->sons_->access(8)->data_;
            auto& aktualnySyn = hierarchy.emplaceSon(currentKraj, zahrCounter);

            aktualnySyn.data_.nastavAtributy(uj);
            aktualnySyn.data_.level = 2;
            ++zahrCounter;
            continue;
        }
        int krajIndex = std::stoi(uj.note.substr(0, 1)) - 1;
        int sonIndex = std::stoi(uj.note.substr(uj.note.length() - 2, 2)) - 1;
        auto& currentKraj = *hierarchy.accessRoot()->sons_->access(krajIndex)->data_;
        auto& aktualnySyn = hierarchy.emplaceSon(currentKraj, sonIndex);

        aktualnySyn.data_.nastavAtributy(uj);
        aktualnySyn.data_.level = 2;
    }

    citacOkresy.zatvorSubor();
}

void DruhaUroven::NacitajObce()
{
    Citac citacObce("data/obce.csv");
    citacObce.preskocPrvyRiadok();

    while (citacObce.citajRiadok()) {
        UzemnaJednotka uj = citacObce.vytvorUJ();
        int susEndIndex = uj.code.find(uj.note);
        std::string cleanCode = uj.code.substr(3, uj.code.length() - susEndIndex - 4);

        if (uj.code.find("ZZZZZ") != std::string::npos) {
            auto& zahranicieVelaZ = *hierarchy.accessRoot()->sons_->access(8)->data_->sons_->access(1)->data_;
            auto& aktualnySynZahranicia = hierarchy.emplaceSon(zahranicieVelaZ, 0);
            aktualnySynZahranicia.data_.nastavAtributy(uj);
            aktualnySynZahranicia.data_.level = 3;
            continue;
        }
        else if (uj.code.find("ZZZZ") != std::string::npos) {
            auto& zahranicieMaloZ = *hierarchy.accessRoot()->sons_->access(8)->data_->sons_->access(0)->data_;
            auto& aktualnySynZahranicia = hierarchy.emplaceSon(zahranicieMaloZ, 0);
            aktualnySynZahranicia.data_.nastavAtributy(uj);
            aktualnySynZahranicia.data_.level = 3;
            continue;
        }

        int krajIndex = DekodujIndexKraja(cleanCode);
        int okresIndex = std::stoi(uj.code.substr(5, 1), nullptr, 16) - 1;
        auto& currentOkres = *hierarchy.accessRoot()->sons_->access(krajIndex)->data_->sons_->access(okresIndex)->data_;
        int fuckingIndex = currentOkres.sons_->size() - 1;
        auto& aktualnySyn = hierarchy.emplaceSon(currentOkres, fuckingIndex <= 0 ? 0 : fuckingIndex);

        aktualnySyn.data_.nastavAtributy(uj);
        aktualnySyn.data_.level = 3;
    }

    citacObce.zatvorSubor();
}

size_t DruhaUroven::DekodujIndexKraja(std::string indexStr)
{
    // 8 je index kraju zahranicie, ten nema cisla z ktorych je mozne zistit jeho index
    size_t krajIndex = 8;
    switch (std::stoi(indexStr)) {
    case 10:
        krajIndex = 0;
        break;
    case 21:
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
    return krajIndex;
}
