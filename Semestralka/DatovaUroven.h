#pragma once

#include <libds/amt/implicit_sequence.h>
#include "UzemnaJednotka.h"
#include "Citac.h"
#include "Kraj.h"
#include "Okres.h"
#include "Obec.h"

class DatovaUroven
{
private:
    bool thisIsACopyDoNotDeletePointersVeryImportant = false;
protected:
    template<typename Type>
    using SequenceType = ds::amt::ImplicitSequence<Type>;
    SequenceType<Kraj>* seqKraje_ = nullptr;
    SequenceType<Okres>* seqOkresy_ = nullptr;
    SequenceType<Obec>* seqObce_ = nullptr;
public:
    DatovaUroven() {}

    DatovaUroven(const DatovaUroven& other) {
        this->seqKraje_ = other.seqKraje_;
        this->seqOkresy_ = other.seqOkresy_;
        this->seqObce_ = other.seqObce_;
        this->thisIsACopyDoNotDeletePointersVeryImportant = true;
    }

    ~DatovaUroven() {
        if (this->thisIsACopyDoNotDeletePointersVeryImportant) {
            return;
        }
        delete this->seqKraje_;
        delete this->seqOkresy_;
        delete this->seqObce_;
    }

	DatovaUroven(std::string suborKraje, std::string suborOkresy, std::string suborObce) {
        this->seqKraje_ = new SequenceType<Kraj>();
        this->seqOkresy_ = new SequenceType<Okres>();
        this->seqObce_ = new SequenceType<Obec>();
        this->nacitajDataZoSuboru<Kraj>(suborKraje, [&](Kraj kraj) { seqKraje_->insertLast().data_ = kraj; });
        this->nacitajDataZoSuboru<Okres>(suborOkresy, [&](Okres okres) { seqOkresy_->insertLast().data_ = okres; });
        this->nacitajDataZoSuboru<Obec>(suborObce, [&](Obec obec) { seqObce_->insertLast().data_ = obec; });
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

