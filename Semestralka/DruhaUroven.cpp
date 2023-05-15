#include "DruhaUroven.h"

DruhaUroven::DruhaUroven(const DatovaUroven& other) : DatovaUroven(other)
{
    VytvorHierarchiu();
}

DruhaUroven::~DruhaUroven()
{
    // koren a oblasti hierarchie alokovane cez new, ostatne su pointre na objekty v implicitnych sekvenciach
    for (auto it = hierarchy.accessRoot()->sons_->begin(); it != hierarchy.accessRoot()->sons_->end(); ++it) {
        delete (*it)->data_;
    }
    delete hierarchy.accessRoot()->data_;
}

void DruhaUroven::VytvorHierarchiu()
{
    hierarchy.emplaceRoot().data_ = new UzemnaJednotka();
    hierarchy.accessRoot()->data_->officialTitle = "Slovensko";

    NacitajKraje();
    NacitajOkresy();
    NacitajObce();
}

void DruhaUroven::RozhranieIteratora()
{
    auto iteratorHierarchie = hierarchy.beginPre();
    this->VypisAktualnuPoziciu(*iteratorHierarchie);

    while (true) {
        this->VypisSynovNaAktualnejPozicii(iteratorHierarchie.dajBlockType().sons_);

        if (this->VstupOdUzivatela(iteratorHierarchie)) {
            break;
        }

        this->VypisAktualnuPoziciu(*iteratorHierarchie);
    }
}

void DruhaUroven::NacitajKraje()
{
    OblastNazov oblastNazov;

    for (auto itKraje = this->seqKraje_->begin(); itKraje != this->seqKraje_->end(); ++itKraje) {
        Kraj& uj = *itKraje;

        if (uj.note.substr(8, 1) == "*") {
            auto& oblastZahranicie = hierarchy.emplaceSon(*hierarchy.accessRoot(), OBLAST_ZAHRANICIE);
            oblastZahranicie.data_ = new UzemnaJednotka();
            oblastZahranicie.data_->officialTitle = oblastNazov.dajNazovOblasti();
            oblastZahranicie.data_->level = 1;

            auto& krajZahranicie = hierarchy.emplaceSon(oblastZahranicie, KRAJ_ZAHRANICIE);
            krajZahranicie.data_ = &uj;
            krajZahranicie.data_->level = 2;
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
            oblast.data_ = new UzemnaJednotka();
            oblast.data_->officialTitle = oblastNazov.dajNazovOblasti();
            oblast.data_->level = 1;
        }

        auto& aktualnySyn = hierarchy.emplaceSon(*hierarchy.accessRoot()->sons_->access(indexOblasti)->data_, indexKraja);
        aktualnySyn.data_ = &uj;
        aktualnySyn.data_->level = 2;
    }
}

void DruhaUroven::NacitajOkresy()
{
    int zahranicieCounter = 0;
    for (auto itOkresy = this->seqOkresy_->begin(); itOkresy != this->seqOkresy_->end(); ++itOkresy) {
        Okres& uj = *itOkresy;

        // 2 okresy zahranicie nemaju note, takze ich treba manualne pridat do kraju zahranicie
        if (uj.note.length() == 0) {
            auto& aktualnySynZahranicie = hierarchy.emplaceSon(vratZahranicieKraj(), zahranicieCounter);

            aktualnySynZahranicie.data_ = &uj;
            aktualnySynZahranicie.data_->level = 3;
            ++zahranicieCounter;
            continue;
        }

        // poradie okresu v ramci jeho kraja su posledne 2 znaky v note
        int indexOkresu = std::stoi(uj.note.substr(1, 2)) - 1;
        auto& aktualnyKraj = vratKraj(uj.code, 3);
        auto& aktualnySyn = hierarchy.emplaceSon(aktualnyKraj, indexOkresu);

        aktualnySyn.data_ = &uj;
        aktualnySyn.data_->level = 3;
    }
}

void DruhaUroven::NacitajObce()
{
    for (auto itObce = this->seqObce_->begin(); itObce != this->seqObce_->end(); ++itObce) {
        Obec& uj = *itObce;

        if (uj.code == "SKZZZZZZZZZZ") {
            auto& zahranicieVelaZ = *vratZahranicieKraj().sons_->access(1)->data_;
            auto& aktualnySynZahranicia = hierarchy.emplaceSon(zahranicieVelaZ, 0);
            aktualnySynZahranicia.data_ = &uj;
            aktualnySynZahranicia.data_->level = 4;
            continue;
        }
        else if (uj.code == "SKZZZZ") {
            auto& zahranicieMaloZ = *vratZahranicieKraj().sons_->access(0)->data_;
            auto& aktualnySynZahranicia = hierarchy.emplaceSon(zahranicieMaloZ, 0);
            aktualnySynZahranicia.data_ = &uj;
            aktualnySynZahranicia.data_->level = 4;
            continue;
        }

        // index okresu je posledne cislo z trojcislia zacinajuce za retazcom SK0, pozor v hexa sustave
        int indexOkresu = std::stoi(uj.code.substr(5, 1), nullptr, 16) - 1;
        auto& aktualnyOkres = *vratKraj(uj.code, 3).sons_->access(indexOkresu)->data_;
        auto& aktualnySyn = hierarchy.emplaceSon(aktualnyOkres, aktualnyOkres.sons_->size());

        aktualnySyn.data_ = &uj;
        aktualnySyn.data_->level = 4;
    }
}

DruhaUroven::HierarchyBlockType& DruhaUroven::vratKraj(std::string identifikator, size_t zaciatok)
{
    int indexOblasti = std::stoi(identifikator.substr(zaciatok, 1)) - 1;
    int indexKraja = std::stoi(identifikator.substr(zaciatok + 1, 1));

    // specialny pripad pre tu oblast, kde je bratislavsky kraj, ta uz ma 0, ostatne idu od 1
    if (indexOblasti != 0) {
        indexKraja -= 1;
    }

    return *hierarchy.accessRoot()->sons_->access(indexOblasti)->data_->sons_->access(indexKraja)->data_;
}

DruhaUroven::HierarchyBlockType& DruhaUroven::vratZahranicieKraj()
{
    return *hierarchy.accessRoot()->sons_->access(OBLAST_ZAHRANICIE)->data_->sons_->access(KRAJ_ZAHRANICIE)->data_;
}

void DruhaUroven::VypisAktualnuPoziciu(DataType uj)
{
    std::cout << "Aktualna pozicia: \x1B[32m" + uj->toString(false) << "\033[0m" << std::endl;
}

void DruhaUroven::VypisSynovNaAktualnejPozicii(ds::amt::IS<HierarchyBlockType*>* synovia)
{
    size_t indexSyna = 0;
    std::cout << "Synovia aktualnej uzemnej jednotky:\n";

    for (auto it = synovia->begin(); it != synovia->end(); ++it) {
        std::cout << (*it)->data_->toString(true, indexSyna) << std::endl;
        ++indexSyna;
    }
}

bool DruhaUroven::VstupOdUzivatela(ds::amt::Hierarchy<HierarchyBlockType>::PreOrderHierarchyIterator& it)
{
    std::string vstup;
    std::cout << "Zadaj moznost [u]p, [s]on (index), [f]ilter (argument) (predicate), [q]uit:\n";
    std::cin >> vstup;
    std::cin.ignore(256, '\n');

    if (vstup.find("u") == 0) {
        --it;
    }
    else if (vstup.find("s") == 0) {
        size_t poradie;
        std::cin >> poradie;
        std::cin.ignore(256, '\n');
        it += poradie;
    }
    else if (vstup.find("f") == 0) {
        this->filtrujHierarchiu(it);
    }
    else if (vstup.find("q") == 0) {
        std::cout << "\x1B[31mKoniec\033[0m\n";
        return true;
    }
    else {
        std::cout << "\x1B[31mNeznama moznost\033[0m\n";
    }

    std::cout << "\n";
    return false;
}

void DruhaUroven::filtrujHierarchiu(ds::amt::Hierarchy<HierarchyBlockType>::PreOrderHierarchyIterator it)
{
    using ResultSequence = ds::amt::ImplicitSequence<DataType>;
    ResultSequence vystupnaSekvencia;
    std::string subString;
    std::cin >> subString;
    std::cin.ignore(256, '\n');

    std::function<bool(DataType)> lambdaContains = [subString](const DataType& uj) { return uj->officialTitle.find(subString) != std::string::npos; };
    std::function<bool(DataType)> lambdaStartsWith = [subString](const DataType& uj) { return uj->officialTitle.find(subString) == 0; };
    std::function<bool(DataType)> lambdaHasTypeKraj = [](const DataType& uj) { return dynamic_cast<Kraj*>(uj) != nullptr; };
    std::function<bool(DataType)> lambdaHasTypeOkres = [](const DataType& uj) { return dynamic_cast<Okres*>(uj) != nullptr; };
    std::function<bool(DataType)> lambdaHasTypeObec = [](const DataType& uj) { return dynamic_cast<Obec*>(uj) != nullptr; };
    std::function<bool(DataType)> lambdaVsetko = [](const DataType& uj) { return true; };
    std::function<bool(DataType)> aktualnaLambda;

    std::string predikat;
    std::cin >> predikat;
    std::cin.ignore(256, '\n');

    if (predikat.find("c") == 0) {
        aktualnaLambda = lambdaContains;
    }
    else if (predikat.find("s") == 0) {
        aktualnaLambda = lambdaStartsWith;
    }
    else if (predikat.find("h") == 0) {
        if (subString.find("kraj") == 0) {
            aktualnaLambda = lambdaHasTypeKraj;
        }
        else if (subString.find("okres") == 0) {
            aktualnaLambda = lambdaHasTypeOkres;
        }
        else if (subString.find("obec") == 0) {
            aktualnaLambda = lambdaHasTypeObec;
        }
        else {
            std::cout << "\x1B[31mNespravny typ\033[0m\n";
            return;
        }
    }
    else if (predikat.find("a") == 0) {
        aktualnaLambda = lambdaVsetko;
    }
    else {
        std::cout << "\x1B[31mNespravna moznost\033[0m\n";
        return;
    }

    auto itBegin = ds::amt::MultiWayExplicitHierarchy<DataType>::PreOrderHierarchyIterator(&hierarchy, &it.dajBlockType());
    auto itEnd = ds::amt::MultiWayExplicitHierarchy<DataType>::PreOrderHierarchyIterator(&hierarchy, nullptr);

    Algorithm::findElementsWithProperty(
        itBegin,
        itEnd,
        aktualnaLambda,
        vystupnaSekvencia,
            [](auto& result, auto data) {
            result.insertLast().data_ = data;
        });

    std::cout << "Vyber utriedenie [a]lphabetical sort, [v]owelsCount sort, [d]o not sort:\n";
    std::string utried;
    std::cin >> utried;
    std::cin.ignore(256, '\n');

    if (utried.find("a") == 0) {
        std::locale skLocale("Slovak_Slovakia.1250");
        ds::adt::QuickSort<DataType> quickSort;
        quickSort.sort(vystupnaSekvencia, [&](DataType fst, DataType snd) { return skLocale(fst->officialTitle, snd->officialTitle); });
    }

    for (auto itSeq = vystupnaSekvencia.begin(); itSeq != vystupnaSekvencia.end(); ++itSeq) {
        std::cout << "\x1B[33m" << (*itSeq)->toString() << "\033[0m" << std::endl;
    }

    std::cout << "Pocet vysledkov: " << vystupnaSekvencia.size() << "\n";
}
