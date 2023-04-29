#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_sequence.h>

namespace ds::adt {

    template <typename T>
    class List :
        virtual public ADT
    {
    public:
        virtual size_t calculateIndex(T element) = 0;

        virtual bool contains(T element) = 0;

        virtual T accessFirst() = 0;
        virtual T accessLast() = 0;
        virtual T access(size_t index) = 0;

        virtual void insertFirst(T element) = 0;
        virtual void insertLast(T element) = 0;
        virtual void insert(T element, size_t index) = 0;

        virtual void set(size_t index, T element) = 0;

        virtual void removeFirst() = 0;
        virtual void removeLast() = 0;
        virtual void remove(size_t index) = 0;
    };

    //----------

    template <typename T, typename SequenceType>
    class GeneralList :
        public List<T>,
        public ADS<T>
    {
    public:
        using IteratorType = typename SequenceType::IteratorType;

    public:
        GeneralList();
        GeneralList(const GeneralList& other);

        size_t calculateIndex(T element) override;

        bool contains(T element) override;

        T accessFirst() override;
        T accessLast() override;
        T access(size_t index) override;

        void insertFirst(T element) override;
        void insertLast(T element) override;
        void insert(T element, size_t index) override;

        void set(size_t index, T element) override;

        void removeFirst() override;
        void removeLast() override;
        void remove(size_t index) override;

        IteratorType begin();
        IteratorType end();

    protected:
        SequenceType* getSequence() const;
    };

    //----------

    template <typename T>
    class ImplicitList :
        public GeneralList<T, amt::IS<T>>
    {
    };

    //----------

    template <typename T>
    class ImplicitCyclicList :
        public GeneralList<T, amt::CIS<T>>
    {
    };

    //----------

    template <typename T>
    class SinglyLinkedList :
        public GeneralList<T, amt::SinglyLS<T>>
    {
    };

    //----------

    template <typename T>
    class SinglyCyclicLinkedList :
        public GeneralList<T, amt::SinglyCLS<T>>
    {
    };

    //----------

    template <typename T>
    class DoublyLinkedList :
        public GeneralList<T, amt::DoublyLS<T>>
    {
    };

    //----------

    template <typename T>
    class DoublyCyclicLinkedList :
        public GeneralList<T, amt::DoublyCLS<T>>
    {
    };

    //----------

    template<typename T, typename SequenceType>
    GeneralList<T, SequenceType>::GeneralList() :
        ADS<T>(new SequenceType())
    {
    }

    template<typename T, typename SequenceType>
    GeneralList<T, SequenceType>::GeneralList(const GeneralList& other) :
        ADS<T>(new SequenceType(), other)
    {
    }

    template<typename T, typename SequenceType>
    size_t GeneralList<T, SequenceType>::calculateIndex(T element)
    {
        size_t result = 0;
        typename SequenceType::BlockType* block = this->getSequence()->findBlockWithProperty(
            [&](typename SequenceType::BlockType* b)
            {
                if (b->data_ == element)
                {
                    return true;
                }
                else
                {
                    result++;
                    return false;
                }
            });
        return block != nullptr ? result : INVALID_INDEX;
    }

    template<typename T, typename SequenceType>
    bool GeneralList<T, SequenceType>::contains(T element)
    {
        return this->calculateIndex(element) != INVALID_INDEX;
    }

    template<typename T, typename SequenceType>
    T GeneralList<T, SequenceType>::accessFirst()
    {
        typename SequenceType::BlockType* block = this->getSequence()->accessFirst();

        if (block == nullptr)
        {
            throw std::out_of_range("List is empty!");
        }

        return block->data_;
    }

    template<typename T, typename SequenceType>
    T GeneralList<T, SequenceType>::accessLast()
    {
        typename SequenceType::BlockType* block = this->getSequence()->accessLast();

        if (block == nullptr)
        {
            throw std::out_of_range("List is empty!");
        }

        return block->data_;
    }

    template<typename T, typename SequenceType>
    T GeneralList<T, SequenceType>::access(size_t index)
    {
        typename SequenceType::BlockType* block = this->getSequence()->access(index);

        if (block == nullptr)
        {
            throw std::out_of_range("Invalid index!");
        }

        return block->data_;
    }

    template<typename T, typename SequenceType>
    void GeneralList<T, SequenceType>::insertFirst(T element)
    {
        this->getSequence()->insertFirst().data_ = element;
    }

    template<typename T, typename SequenceType>
    void GeneralList<T, SequenceType>::insertLast(T element)
    {
        this->getSequence()->insertLast().data_ = element;
    }

    template<typename T, typename SequenceType>
    void GeneralList<T, SequenceType>::insert(T element, size_t index)
    {
        if (index > this->size())
        {
            throw std::out_of_range("Invalid index!");
        }

        this->getSequence()->insert(index).data_ = element;
    }

    template<typename T, typename SequenceType>
    void GeneralList<T, SequenceType>::set(size_t index, T element)
    {
        typename SequenceType::BlockType* block = this->getSequence()->access(index);

        if (block == nullptr)
        {
            throw std::out_of_range("Invalid index!");
        }

        block->data_ = element;
    }

    template<typename T, typename SequenceType>
    void GeneralList<T, SequenceType>::removeFirst()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("List is empty!");
        }
        this->getSequence()->removeFirst();
    }

    template<typename T, typename SequenceType>
    void GeneralList<T, SequenceType>::removeLast()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("List is empty!");
        }
        this->getSequence()->removeLast();
    }

    template<typename T, typename SequenceType>
    void GeneralList<T, SequenceType>::remove(size_t index)
    {
        if (index >= this->size())
        {
            throw std::out_of_range("Invalid index!");
        }
        this->getSequence()->remove(index);
    }

    template <typename T, typename SequenceType>
    auto GeneralList<T, SequenceType>::begin() -> IteratorType
    {
        return this->getSequence()->begin();
    }

    template <typename T, typename SequenceType>
    auto GeneralList<T, SequenceType>::end() -> IteratorType
    {
        return this->getSequence()->end();
    }

    template<typename T, typename SequenceType>
    SequenceType* GeneralList<T, SequenceType>::getSequence() const
    {
        return dynamic_cast<SequenceType*>(this->memoryStructure_);
    }
}