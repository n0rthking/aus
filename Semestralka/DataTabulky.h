#pragma once

#include "UzemnaJednotka.h"
#include <libds/amt/implicit_sequence.h>

template <typename DataType>
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
		this->pridajData(newData);
	}

	void pridajDoSekvencie(DataTabulky* novy) {
		if (novy != nullptr) {
			this->pridajData(novy->data_);
		}
	}

	void vypis() {
		if (this->pocet_ == 1) {
			std::cout << this->data_->toString() << " " << this->data_->code << "\n";
		}
		else if (this->pocet_ > 1) {
			for (auto it = this->sekvencia_->begin(); it != this->sekvencia_->end(); ++it) {
				std::cout << (*it)->toString() << " " << (*it)->code << "\n";
			}
			std::cout << "\n";
		}
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

