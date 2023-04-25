#pragma once

#include <libds/amt/implicit_sequence.h>
#include "UzemnaJednotka.h"
#include "Citac.h"
#include <string>

template<typename Type>
using SequenceType = ds::amt::ImplicitSequence<Type>;

class DatovaUroven
{
protected:
    SequenceType<Kraj> seqKraje_;
    SequenceType<Okres> seqOkresy_;
    SequenceType<Obec> seqObce_;
public:
	DatovaUroven() {
        this->nacitajDataZoSuboru<Kraj>("kraje.csv", [&](Kraj kraj) { seqKraje_.insertLast().data_ = kraj; });
        this->nacitajDataZoSuboru<Okres>("okresy.csv", [&](Okres okres) { seqOkresy_.insertLast().data_ = okres; });
        this->nacitajDataZoSuboru<Obec>("obce.csv", [&](Obec obec) { seqObce_.insertLast().data_ = obec; });
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

