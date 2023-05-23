#pragma once

#include <fstream>
#include <sstream>

template<typename DataType>
class Citac
{
private:
	std::ifstream suborStream;
	std::string aktualnyRiadok = "";
public:
	Citac(std::string nazovSuboru) {
		suborStream.open(nazovSuboru);
	}

	bool skontrolujSubor()
	{
		return suborStream.is_open();
	}

	void preskocPrvyRiadok()
	{
		std::string obsah;
		std::getline(suborStream, obsah);
	}

	bool citajRiadok()
	{
		return !std::getline(suborStream, aktualnyRiadok).fail();
	}

	DataType* vytvorUJ()
	{
		std::string hodnota;
		std::stringstream strstrm(aktualnyRiadok);
		DataType* uj = new DataType();

		std::getline(strstrm, hodnota, ';');
		uj->sortNumber = std::stoi(hodnota);

		std::getline(strstrm, hodnota, ';');
		uj->code = hodnota;

		std::getline(strstrm, hodnota, ';');
		uj->officialTitle = hodnota;

		std::getline(strstrm, hodnota, ';');
		uj->mediumTitle = hodnota;

		std::getline(strstrm, hodnota, ';');
		uj->shortTitle = hodnota;

		std::getline(strstrm, hodnota, ';');
		uj->note = hodnota;

		return uj;
	}

	void zatvorSubor()
	{
		suborStream.close();
	}
};

