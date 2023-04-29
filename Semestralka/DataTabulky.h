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

	DataTabulky() {}

	DataTabulky(DataType* newData) {
		this->dolezitaMetoda(newData);
	}

	void dalsiaMetoda(DataTabulky* novy) {
		if (novy != nullptr) {
			this->dolezitaMetoda(novy->data_);
		}
	}

	void vypis() {
		if (this->sekvencia_ == nullptr) {
			std::cout << this->data_->toString() << " " << this->data_->code << "\n";
		}
		else {
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
	int dolezitaMetoda(DataType* newData) {
		if (this->data_ == nullptr && this->sekvencia_ == nullptr) {
			this->data_ = newData;
			return 1;
		}

		if (this->data_ != nullptr) {
			this->sekvencia_ = new Sekvencia();
			this->sekvencia_->insertLast().data_ = this->data_;
			this->data_ = nullptr;
			this->sekvencia_->insertLast().data_ = newData;
			return 2;
		}

		if (this->sekvencia_ != nullptr) {
			this->sekvencia_->insertLast().data_ = newData;
			return 3;
		}

		return 0;
	}
};

