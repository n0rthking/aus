#include "TretiaUroven.h"

TretiaUroven::TretiaUroven()
{
	for (auto it = this->seqObce_.begin(); it != this->seqObce_.end(); ++it) {
		Obec* uj = &(*it);
		DataTabulky<Obec>* dataTab = new DataTabulky<Obec>(uj);
		tabulka.insert(uj->officialTitle, dataTab);
	}

	/*for (auto it = this->tabulka.begin(); it != this->tabulka.end(); ++it) {
		auto& itDeref = *it;
		if (itDeref.key_.find("Vi") == std::string::npos) {
			continue;
		}
		std::cout << itDeref.key_ << "\n";
		itDeref.data_->vypis();
		std::cout << "-------\n";
	}*/

	std::string kluc;
	std::cout << "Zadaj kluc: ";
	std::cin >> kluc;
	DataTabulky<Obec>** vysledok;
	bool ok = tabulka.tryFind(kluc, vysledok);
	if (ok) {
		(*vysledok)->vypis();
	}
}
