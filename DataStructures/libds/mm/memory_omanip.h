#pragma once

#include <ostream>
#include <iomanip>
#include <sstream>

namespace ds::mm {

	template<typename DataType>
	class PtrPrint {
	public:
		PtrPrint(const DataType* ptr, int base, int width);

		const DataType* ptr_;
		std::ostringstream oss_;
	};

	template<typename DataType>
	class PtrPrintBin : public PtrPrint<DataType> {
	public:
		PtrPrintBin(const DataType* ptr);
	};

	template<typename DataType>
    PtrPrint<DataType>::PtrPrint(const DataType* ptr, int base, int width) :
		ptr_(ptr)
	{
		oss_ << std::setbase(base) << std::setw(width);

		unsigned char* ptrB = (unsigned char*)(void*)ptr_;

		for (size_t byte = 0; byte < sizeof(DataType); byte++, ptrB++) {
			oss_ << "|" << (int)*ptrB;
		}
		oss_ << "|";
	}

	template<typename DataType>
    PtrPrintBin<DataType>::PtrPrintBin(const DataType* ptr) :
		PtrPrint<DataType>(ptr, 2, 8)
	{
	}

}

template<typename DataType>
std::ostream& operator<<(std::ostream& os, const ds::mm::PtrPrint<DataType>& pp) {
	return os << pp.oss_.str();
}
