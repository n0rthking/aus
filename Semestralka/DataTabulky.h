#pragma once

#include "UzemnaJednotka.h"
#include <libds/amt/implicit_sequence.h>

template<typename DataType>
class DataTabulky
{
private:
	using Sekvencia = ds::amt::ImplicitSequence<DataType*>;
public:
	DataType* data_ = nullptr;
	Sekvencia* sekvencia_ = nullptr;
	size_t pocet_ = 0;

	DataTabulky() {}

	DataTabulky(DataType* newData) {
		if (newData != nullptr) {
			this->pridajData(newData);
		}
	}

	void pridajDoSekvencie(const DataTabulky& novy) {
		this->pridajData(novy.data_);
	}

	void vypis() {
		std::cout << "----------\n";
		if (this->pocet_ == 1) {
			std::cout << this->data_->fullInfo() << "\n";
		}
		else if (this->pocet_ > 1) {
			size_t aktualnyPocet = 0;
			for (auto it = this->sekvencia_->begin(); it != this->sekvencia_->end(); ++it) {
				std::cout << (*it)->fullInfo() << "\n";
				++aktualnyPocet;
				if (aktualnyPocet < this->sekvencia_->size()) {
					std::cout << "---\n";
				}
			}
		}
		std::cout << "----------\n";
	}

	~DataTabulky() {
		delete this->sekvencia_;
	}
private:
	void pridajData(DataType* newData) {
		if (this->pocet_ == 0) {
			this->data_ = newData;
			++this->pocet_;
			return;
		}

		if (this->pocet_ == 1) {
			this->sekvencia_ = new Sekvencia();
			this->sekvencia_->insertLast().data_ = this->data_;
			this->data_ = nullptr;
		}

		this->sekvencia_->insertLast().data_ = newData;
		++this->pocet_;
	}
};

