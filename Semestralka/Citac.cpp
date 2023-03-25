#include "Citac.h"

bool Citac::skontrolujSubor()
{
	return suborStream.is_open();
}

void Citac::preskocPrvyRiadok()
{
	std::string obsah;
	std::getline(suborStream, obsah);
}

bool Citac::citajRiadok()
{
	return !std::getline(suborStream, aktualnyRiadok).fail();
}

UzemnaJednotka Citac::vytvorUJ()
{
	std::string hodnota;
	std::stringstream strstrm(aktualnyRiadok);
	UzemnaJednotka uj;

	std::getline(strstrm, hodnota, ';');
	uj.sortNumber = std::stoi(hodnota);

	std::getline(strstrm, hodnota, ';');
	uj.code = hodnota;

	std::getline(strstrm, hodnota, ';');
	uj.officialTitle = hodnota;

	std::getline(strstrm, hodnota, ';');
	uj.mediumTitle = hodnota;

	std::getline(strstrm, hodnota, ';');
	uj.shortTitle = hodnota;

	std::getline(strstrm, hodnota, ';');
	uj.note = hodnota;

	return uj;
}

void Citac::zatvorSubor()
{
	suborStream.close();
}
