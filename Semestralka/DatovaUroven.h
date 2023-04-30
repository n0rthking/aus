#pragma once

#include <libds/amt/implicit_sequence.h>
#include "UzemnaJednotka.h"
#include "Citac.h"
#include <string>

class DatovaUroven
{
protected:
    template<typename Type>
    using SequenceType = ds::amt::ImplicitSequence<Type>;
    SequenceType<Kraj>* seqKraje_ = nullptr;
    SequenceType<Okres>* seqOkresy_ = nullptr;
    SequenceType<Obec>* seqObce_ = nullptr;
public:
    DatovaUroven() {}

    DatovaUroven(const DatovaUroven& other) {
        std::cout << "[debug] copy constructor called\n";
        this->seqKraje_ = other.seqKraje_;
        this->seqOkresy_ = other.seqOkresy_;
        this->seqObce_ = other.seqObce_;
    }

	DatovaUroven(bool xyz) {
        std::cout << "[debug] nacitaj data konstruktor\n";
        this->seqKraje_ = new SequenceType<Kraj>();
        this->seqOkresy_ = new SequenceType<Okres>();
        this->seqObce_ = new SequenceType<Obec>();
        this->nacitajDataZoSuboru<Kraj>("kraje.csv", [&](Kraj kraj) { seqKraje_->insertLast().data_ = kraj; });
        this->nacitajDataZoSuboru<Okres>("okresy.csv", [&](Okres okres) { seqOkresy_->insertLast().data_ = okres; });
        this->nacitajDataZoSuboru<Obec>("obce.csv", [&](Obec obec) { seqObce_->insertLast().data_ = obec; });
	}
private:
    template<typename DataType, typename LambdaType>
    bool nacitajDataZoSuboru(std::string nazovSuboru, LambdaType insertFunction) {
        Citac<DataType> citac("data/" + nazovSuboru);

        if (!citac.skontrolujSubor()) {
            return false;
        }

        citac.preskocPrvyRiadok();

        while (citac.citajRiadok()) {
            insertFunction(citac.vytvorUJ());
        }

        citac.zatvorSubor();
        return true;
	}
};

