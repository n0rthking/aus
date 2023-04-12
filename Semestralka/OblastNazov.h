#pragma once

#include <string>
#include <libds/amt/implicit_sequence.h>

class OblastNazov
{
private:
	ds::amt::ImplicitSequence<std::string> nazvy;
	size_t pocitadlo = 0;
public:
	OblastNazov() {
		nazvy.insertLast().data_ = "Oblast Bratislava";
		nazvy.insertLast().data_ = "Oblast Zapad";
		nazvy.insertLast().data_ = "Oblast Stred";
		nazvy.insertLast().data_ = "Oblast Vychod";
		nazvy.insertLast().data_ = "Oblast Zahranicie";
	}

	std::string dajNazovOblasti() {
		std::string aktualny = nazvy.access(pocitadlo)->data_;
		++pocitadlo;
		return aktualny;
	}
};

