#include "DruhaUroven.h"

DruhaUroven::DruhaUroven()
{
    hierarchy.emplaceRoot().data_.officialTitle = "Slovensko";

    VytvorHierarchiu();
    RozhranieIteratora();

    /*for (auto it = hierarchy.beginPre(); it != hierarchy.endPre(); ++it) {
        std::cout << (*it).toString() << std::endl;
    }*/

    /*ds::amt::ImplicitSequence<DataType> resultSeq;
    Algorithm<DataType, ds::amt::ImplicitSequence<DataType>> algorithm;
    std::string subString = "l";

    auto itBegin = ds::amt::MultiWayExplicitHierarchy<DataType>::PreOrderHierarchyIterator(&hierarchy, hierarchy.accessRoot()->sons_->access(2)->data_);
    auto itEnd = ds::amt::MultiWayExplicitHierarchy<DataType>::PreOrderHierarchyIterator(&hierarchy, nullptr);

    algorithm.findElementsWithProperty(
        itBegin,
        itEnd,
        [subString](auto uj) {
            return uj.typ == TYP_KRAJ || uj.typ == TYP_OBEC;
        },
        resultSeq,
            [](auto& result, auto data) {
            result.insertLast().data_ = data;
        });

    for (const auto& element : resultSeq) {
        std::cout << element.toStr() << std::endl;
    }*/
}

void DruhaUroven::VytvorHierarchiu()
{
    NacitajKraje();
    NacitajOkresy();
    NacitajObce();
}

void DruhaUroven::RozhranieIteratora()
{
    auto iteratorHierarchie = hierarchy.beginPre();
    this->VypisAktualnuPoziciu(*iteratorHierarchie);

    while (true) {
        this->VypisSynovNaAktualnejPozicii(iteratorHierarchie.allData().sons_);

        if (this->VstupOdUzivatela(iteratorHierarchie)) {
            break;
        }

        this->VypisAktualnuPoziciu(*iteratorHierarchie);
    }
}

void DruhaUroven::NacitajKraje()
{
    Citac<Kraj> citacKraje("data/kraje.csv");
    citacKraje.preskocPrvyRiadok();

    while (citacKraje.citajRiadok()) {
        Kraj uj = citacKraje.vytvorUJ();

        if (uj.note.substr(8, 1) == "*") {
            auto& oblastZahranicie = hierarchy.emplaceSon(*hierarchy.accessRoot(), OBLAST_ZAHRANICIE);
            oblastZahranicie.data_.officialTitle = "Oblast";
            oblastZahranicie.data_.level = 1;

            auto& krajZahranicie = hierarchy.emplaceSon(oblastZahranicie, KRAJ_ZAHRANICIE);
            krajZahranicie.data_ = uj;
            krajZahranicie.data_.level = 2;
            continue;
        }

        int indexOblasti = std::stoi(uj.note.substr(8, 1)) - 1;
        int indexKraja = std::stoi(uj.note.substr(9, 1));

        // uprava indexov pre vsetky okrem toho kde je bratislavsky kraj
        if (indexOblasti != 0) {
            indexKraja -= 1;
        }

        // pre prvy kraj z danej oblasti sa najprv vytvori oblast
        if (indexKraja == 0) {
            auto& oblast = hierarchy.emplaceSon(*hierarchy.accessRoot(), indexOblasti);
            oblast.data_.officialTitle = "Oblast";
            oblast.data_.level = 1;
        }

        auto& aktualnySyn = hierarchy.emplaceSon(*hierarchy.accessRoot()->sons_->access(indexOblasti)->data_, indexKraja);
        aktualnySyn.data_ = uj;
        aktualnySyn.data_.level = 2;
    }

    citacKraje.zatvorSubor();
}

void DruhaUroven::NacitajOkresy()
{
    Citac<Okres> citacOkresy("data/okresy.csv");
    citacOkresy.preskocPrvyRiadok();

    int zahranicieCounter = 0;
    while (citacOkresy.citajRiadok()) {
        Okres uj = citacOkresy.vytvorUJ();

        // 2 okresy zahranicie nemaju note, takze ich treba manualne pridat do kraju zahranicie
        if (uj.note.length() == 0) {
            auto& aktualnySynZahranicie = hierarchy.emplaceSon(vratZahranicieKraj(), zahranicieCounter);

            aktualnySynZahranicie.data_ = uj;
            aktualnySynZahranicie.data_.level = 3;
            ++zahranicieCounter;
            continue;
        }

        // poradie okresu v ramci jeho kraja su posledne 2 znaky v note
        int indexOkresu = std::stoi(uj.note.substr(1, 2)) - 1;
        auto& aktualnyKraj = vratKraj(uj.code, 3);
        auto& aktualnySyn = hierarchy.emplaceSon(aktualnyKraj, indexOkresu);

        aktualnySyn.data_ = uj;
        aktualnySyn.data_.level = 3;
    }

    citacOkresy.zatvorSubor();
}

void DruhaUroven::NacitajObce()
{
    Citac<Obec> citacObce("data/obce.csv");
    citacObce.preskocPrvyRiadok();

    while (citacObce.citajRiadok()) {
        Obec uj = citacObce.vytvorUJ();

        if (uj.code.find("ZZZZZ") != std::string::npos) {
            auto& zahranicieVelaZ = *vratZahranicieKraj().sons_->access(1)->data_;
            auto& aktualnySynZahranicia = hierarchy.emplaceSon(zahranicieVelaZ, 0);
            aktualnySynZahranicia.data_ = uj;
            aktualnySynZahranicia.data_.level = 4;
            continue;
        }
        else if (uj.code.find("ZZZZ") != std::string::npos) {
            auto& zahranicieMaloZ = *vratZahranicieKraj().sons_->access(0)->data_;
            auto& aktualnySynZahranicia = hierarchy.emplaceSon(zahranicieMaloZ, 0);
            aktualnySynZahranicia.data_ = uj;
            aktualnySynZahranicia.data_.level = 4;
            continue;
        }

        // index okresu je posledne cislo z trojcislia zacinajuce za retazcom SK0, pozor v hexa sustave
        int indexOkresu = std::stoi(uj.code.substr(5, 1), nullptr, 16) - 1;
        auto& aktualnyOkres = *vratKraj(uj.code, 3).sons_->access(indexOkresu)->data_;
        int poradieObce = aktualnyOkres.sons_->size() - 1;
        auto& aktualnySyn = hierarchy.emplaceSon(aktualnyOkres, (poradieObce <= 0) ? 0 : poradieObce);

        aktualnySyn.data_ = uj;
        aktualnySyn.data_.level = 4;
    }

    citacObce.zatvorSubor();
}

BlockResultType& DruhaUroven::vratKraj(std::string identifikator, size_t zaciatok)
{
    int indexOblasti = std::stoi(identifikator.substr(zaciatok, 1)) - 1;
    int indexKraja = std::stoi(identifikator.substr(zaciatok + 1, 1));

    // specialny pripad pre tu oblast, kde je bratislavsky kraj, ta uz ma 0, ostatne idu od 1
    if (indexOblasti != 0) {
        indexKraja -= 1;
    }

    return *hierarchy.accessRoot()->sons_->access(indexOblasti)->data_->sons_->access(indexKraja)->data_;
}

BlockResultType& DruhaUroven::vratZahranicieKraj()
{
    return *hierarchy.accessRoot()->sons_->access(OBLAST_ZAHRANICIE)->data_->sons_->access(KRAJ_ZAHRANICIE)->data_;
}

void DruhaUroven::VypisAktualnuPoziciu(DataType uj)
{
    std::cout << "Aktualna pozicia: " + uj.toString(false) << std::endl;
}

void DruhaUroven::VypisSynovNaAktualnejPozicii(ds::amt::IS<BlockResultType*>* synovia)
{
    size_t indexSyna = 0;
    std::cout << "Synovia aktualnej uzemnej jednotky:\n";

    for (auto it = synovia->begin(); it != synovia->end(); ++it) {
        std::cout << (*it)->data_.toString(true, indexSyna) << std::endl;
        ++indexSyna;
    }
}

bool DruhaUroven::VstupOdUzivatela(ds::amt::Hierarchy<BlockResultType>::PreOrderHierarchyIterator& it)
{
    std::string vstup;
    std::cout << "Zadaj moznost [u]p, [s]on (index), [q]uit: ";
    std::cin >> vstup;

    if (vstup.find("u") != std::string::npos) {
        --it;
    }
    else if (vstup.find("s") != std::string::npos) {
        size_t poradie;
        std::cin >> poradie;
        it += poradie;
    }
    else {
        std::cout << "Koniec\n";
        return true;
    }

    std::cout << "\n";
    return false;
}