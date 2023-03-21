#pragma once

#include <libds/mm/memory_manager.h>
#include <libds/mm/memory_omanip.h>
#include <libds/constants.h>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <algorithm>
#include <iostream>

namespace ds::mm {

    template<typename BlockType>
    class CompactMemoryManager : public MemoryManager<BlockType> {
    public:
        CompactMemoryManager();
        CompactMemoryManager(size_t size);
        CompactMemoryManager(const CompactMemoryManager<BlockType>& other);
        ~CompactMemoryManager() override;

        BlockType* allocateMemory() override;
        BlockType* allocateMemoryAt(size_t index);
        void releaseMemory(BlockType* pointer) override;
        void releaseMemoryAt(size_t index);
        void releaseMemory();

        size_t getCapacity() const;

        CompactMemoryManager<BlockType>& assign(const CompactMemoryManager<BlockType>& other);
        void changeCapacity(size_t newCapacity);
        void shrinkMemory();
        void clear();
        bool equals(const CompactMemoryManager<BlockType>& other) const;
        void* calculateAddress(const BlockType& data);
        size_t calculateIndex(const BlockType& data);
        BlockType& getBlockAt(size_t index);
        void swap(size_t index1, size_t index2);

        void print(std::ostream& os);

    private:
        size_t getAllocatedBlocksSize() const;
        size_t getAllocatedCapacitySize() const;

    private:
        BlockType* base_;
        BlockType* end_;
        BlockType* limit_;

        static const size_t INIT_SIZE = 4;
    };

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager() :
        CompactMemoryManager(INIT_SIZE)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(size_t size) :
        base_(static_cast<BlockType*>(std::calloc(size, sizeof(BlockType)))),
        end_(base_),
        limit_(base_ + size)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(const CompactMemoryManager<BlockType>& other):
        CompactMemoryManager(other.getAllocatedBlockCount())
    {
        this->assign(other);
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::~CompactMemoryManager()
    {
        // Ensures that destructor for each object is called.
        CompactMemoryManager<BlockType>::releaseMemory(base_);
        std::free(base_);

        base_ = nullptr;
        end_ = nullptr;
        limit_ = nullptr;
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemory()
    {
        return this->allocateMemoryAt(end_ - base_);
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemoryAt(size_t index)
    {
        if (end_ == limit_)
        {
            this->changeCapacity(2 * this->getAllocatedBlockCount());
        }

        if (end_ - base_ > static_cast<std::ptrdiff_t>(index))
        {
            std::memmove(
                base_ + index + 1,
                base_ + index,
                (end_ - base_ - index) * sizeof(BlockType)
            );
        }

        ++MemoryManager<BlockType>::allocatedBlockCount_;
        ++end_;

        return placement_new(base_ + index);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory(BlockType* pointer)
    {
        BlockType* p = pointer;
        while (p != end_)
        {
            p->~BlockType();
            ++p;
        }

        end_ = pointer;
        MemoryManager<BlockType>::allocatedBlockCount_ = end_ - base_;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemoryAt(size_t index)
    {
        this->getBlockAt(index).~BlockType();
        std::memmove(
            base_ + index,
            base_ + index + 1,
            (end_ - base_ - index - 1) * sizeof(BlockType)
        );
        --end_;
        --this->allocatedBlockCount_;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory()
    {
        this->releaseMemory(end_ - 1);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getCapacity() const
    {
        return limit_ - base_;
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>& CompactMemoryManager<BlockType>::assign
        (const CompactMemoryManager<BlockType>& other)
    {
        if (this != &other)
        {
            this->releaseMemory(base_);
            MemoryManager<BlockType>::allocatedBlockCount_ = other.MemoryManager<BlockType>::allocatedBlockCount_;
            void* newBase = std::realloc(base_, other.getAllocatedCapacitySize());
            if (newBase == nullptr)
            {
                throw std::bad_alloc();
            }
            base_ = static_cast<BlockType*>(newBase);
            end_ = base_ + MemoryManager<BlockType>::allocatedBlockCount_;
            limit_ = base_ + (other.limit_ - other.base_);

            for (size_t i = 0; i < other.getAllocatedBlockCount(); ++i)
            {
                placement_copy(base_ + i, *(other.base_ + i));
            }
        }
        return *this;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::shrinkMemory()
    {
        size_t newCapacity = static_cast<size_t>(end_ - base_);

        if (newCapacity < CompactMemoryManager<BlockType>::INIT_SIZE)
        {
            newCapacity = CompactMemoryManager<BlockType>::INIT_SIZE;
        }

        this->changeCapacity(newCapacity);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::changeCapacity(size_t newCapacity)
    {
        if (newCapacity == this->getCapacity())
        {
            return;
        }

        if (newCapacity < this->getAllocatedBlockCount())
        {
            this->releaseMemory(base_ + newCapacity);
        }

        void* newBase = std::realloc(base_, newCapacity * sizeof(BlockType));

        if (newBase == nullptr)
        {
            throw std::bad_alloc();
        }

        base_ = static_cast<BlockType*>(newBase);
        end_ = base_ + MemoryManager<BlockType>::allocatedBlockCount_;
        limit_ = base_ + newCapacity;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::clear()
    {
        this->releaseMemory(base_);
    }

    template<typename BlockType>
    bool CompactMemoryManager<BlockType>::equals(const CompactMemoryManager<BlockType>& other) const
    {
        return this == &other ||
            this->getAllocatedBlockCount() == other.getAllocatedBlockCount() &&
            std::memcmp(base_, other.base_, this->getAllocatedBlocksSize()) == 0;
    }

    template<typename BlockType>
    void* CompactMemoryManager<BlockType>::calculateAddress(const BlockType& data)
    {
        BlockType* p = base_;

        while (p != end_ && p != &data)
        {
            ++p;
        }

        return p == end_ ? nullptr : p;
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::calculateIndex(const BlockType& data)
    {
        return &data < end_ && &data >= base_ ? &data - base_ : INVALID_INDEX;
    }

    template<typename BlockType>
    BlockType& CompactMemoryManager<BlockType>::getBlockAt(size_t index)
    {
        return *(base_ + index);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::swap(size_t index1, size_t index2)
    {
        std::swap(getBlockAt(index1), getBlockAt(index2));
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedBlocksSize() const
    {
        return (end_ - base_) * sizeof(BlockType);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedCapacitySize() const
    {
        return (limit_ - base_) * sizeof(BlockType);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::print(std::ostream& os)
    {
        os << "first = " << base_ << std::endl;
        os << "last = " << end_ << std::endl;
        os << "limit = " << limit_ << std::endl;
        os << "block size = " << sizeof(BlockType) << "B" << std::endl;

        BlockType* ptr = base_;
        while (ptr != limit_)
        {
            std::cout << ptr;
            os << PtrPrintBin<BlockType>(ptr);

            if (ptr == base_) {
                os << "<- first";
            }
            else if (ptr == end_) {
                os << "<- last";
            }
            os << std::endl;
            ++ptr;
        }

        os << limit_ << "|<- limit" << std::endl;
    }

}