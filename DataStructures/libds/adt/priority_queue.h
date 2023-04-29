#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_sequence.h>
#include <libds/amt/implicit_hierarchy.h>
#include <cmath>
#include <functional>

namespace ds::adt {

    template <typename P, typename T>
    struct PriorityQueueItem
    {
        P priority_;
        T data_;

        bool operator==(const PriorityQueueItem<P, T>& other)
        {
            return priority_ == other.priority_ && data_ == other.data_;
        }
    };

    template <typename P, typename T>
    using PQItem = PriorityQueueItem<P, T>;

    //----------

    template <typename P, typename T>
    class PriorityQueue :
        virtual public ADT
    {
    public:
        virtual void push(P priority, T data) = 0;
        virtual T& peek() = 0;
        virtual T pop() = 0;
    };

    template <typename P, typename T>
    using PQ = PriorityQueue<P, T>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class SequencePriorityQueue :
        public PriorityQueue<P, T>,
        public ADS<PQItem<P, T>>
    {
    public:
        SequencePriorityQueue();
        SequencePriorityQueue(const SequencePriorityQueue& other);

        bool equals(const ADT& other) override;

    protected:
        SequenceType* getSequence() const;
    };

    template <typename P, typename T, typename SequenceType>
    using SPQ = SequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class UnsortedSequencePriorityQueue :
        public SequencePriorityQueue<P, T, SequenceType>
    {
    public:
        T& peek() override;

    protected:
        typename SequenceType::BlockType* findHighestPriorityBlock();
    };

    template <typename P, typename T, typename SequenceType>
    using UnsortedSPQ = UnsortedSequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class SortedSequencePriorityQueue :
        public SequencePriorityQueue<P, T, SequenceType>
    {
    public:
        T& peek() override;
        T pop() override;

    protected:
        virtual size_t indexOfHighestPriorityBlock() const = 0;
    };

    template <typename P, typename T, typename SequenceType>
    using SortedSPQ = SortedSequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T>
    class UnsortedImplicitSequencePriorityQueue :
        public UnsortedSequencePriorityQueue<P, T, amt::IS<PQItem<P,T>>>
    {
    public:
        void push(P priority, T data) override;
        T pop() override;

    private:
        using SequenceBlockType = typename amt::IS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using UnsortedISPQ = UnsortedImplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class UnsortedExplicitSequencePriorityQueue :
        public UnsortedSequencePriorityQueue<P, T, amt::SinglyLS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;
        T pop() override;

    private:
        using SequenceBlockType = typename amt::SinglyLS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using UnsortedESPQ = UnsortedExplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class SortedImplicitSequencePriorityQueue :
        public SortedSequencePriorityQueue<P, T, amt::IS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;

    protected:
        size_t indexOfHighestPriorityBlock() const override;

    private:
        using SequenceBlockType = typename amt::IS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using SortedISPQ = SortedImplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class SortedExplicitSequencePriorityQueue :
        public SortedSequencePriorityQueue<P, T, amt::SinglyLS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;

    protected:
        size_t indexOfHighestPriorityBlock() const override;

    private:
        using SequenceBlockType = typename amt::SinglyLS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using SortedESPQ = SortedExplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class TwoLists :
        public AUMS<PQItem<P, T>>,
        public PriorityQueue<P, T>
    {
    public:
        TwoLists(size_t expectedSize);
        TwoLists(const TwoLists<P, T>& other);
        ~TwoLists(); // TODO RAII

        ADT& assign(const ADT& other) override;
        void clear() override;
        size_t size() const override;
        bool isEmpty() const override;
        bool equals(const ADT& other) override;

        void push(P priority, T data) override;
        T& peek() override;
        T pop() override;

    private:
        using ShortSequenceType = amt::IS<PQItem<P, T>>;
        using LongSequenceType = amt::SinglyLS<PQItem<P, T>>;
        using ShortSequenceBlockType = typename ShortSequenceType::BlockType;
        using LongSequenceBlockType = typename LongSequenceType::BlockType;

        ShortSequenceType* shortSequence_;
        LongSequenceType* longSequence_;
    };

    //----------

    template <typename P, typename T>
    class BinaryHeap :
        public PriorityQueue<P, T>,
        public ADS<PQItem<P, T>>
    {
    public:
        BinaryHeap();
        BinaryHeap(const BinaryHeap& other);
        void push(P priority, T data) override;
        T& peek() override;
        T pop() override;

    private:
        using HierarchyBlockType = typename amt::BinaryIH<PQItem<P, T>>::BlockType;
        amt::BinaryIH<PQItem<P, T>>* getHierarchy();
    };

    //----------

    template<typename P, typename T, typename SequenceType>
    SequencePriorityQueue<P, T, SequenceType>::SequencePriorityQueue() :
        ADS<PQItem<P, T>>(new SequenceType())
    {
    }

    template<typename P, typename T, typename SequenceType>
    SequencePriorityQueue<P, T, SequenceType>::SequencePriorityQueue(const SequencePriorityQueue& other) :
        ADS<PQItem<P, T>>(new SequenceType(), other)
    {
    }

    template <typename P, typename T, typename SequenceType>
    bool SequencePriorityQueue<P, T, SequenceType>::equals(const ADT&)
    {
        throw std::logic_error("Unsupported operation!");
    }


    template<typename P, typename T, typename SequenceType>
    SequenceType* SequencePriorityQueue<P, T, SequenceType>::getSequence() const
    {
        return dynamic_cast<SequenceType*>(this->memoryStructure_);
    }

    template<typename P, typename T, typename SequenceType>
    T& UnsortedSequencePriorityQueue<P, T, SequenceType>::peek()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("Queue is empty!");
        }

        return this->findHighestPriorityBlock()->data_.data_;
    }

    template<typename P, typename T, typename SequenceType>
    typename SequenceType::BlockType* UnsortedSequencePriorityQueue<P, T, SequenceType>::findHighestPriorityBlock()
    {
        typename SequenceType::BlockType* bestBlock = this->getSequence()->accessFirst();

        this->getSequence()->processBlocksForward(this->getSequence()->accessNext(*bestBlock), [&](typename SequenceType::BlockType* b)
            {
                if (bestBlock->data_.priority_ > b->data_.priority_)
                {
                    bestBlock = b;
                }
            });

        return bestBlock;
    }

    template<typename P, typename T, typename SequenceType>
    T& SortedSequencePriorityQueue<P, T, SequenceType>::peek()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("Queue is empty!");
        }

        return this->getSequence()->access(this->indexOfHighestPriorityBlock())->data_.data_;
    }

    template<typename P, typename T, typename SequenceType>
    T SortedSequencePriorityQueue<P, T, SequenceType>::pop()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("Queue is empty!");
        }

        size_t index = this->indexOfHighestPriorityBlock();
        T result = this->getSequence()->access(index)->data_.data_;
        this->getSequence()->remove(index);
        return result;
    }

    template<typename P, typename T>
    void UnsortedImplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        PQItem<P, T>& queueData = this->getSequence()->insertLast().data_;
        queueData.priority_ = priority;
        queueData.data_ = data;
    }

    template<typename P, typename T>
    T UnsortedImplicitSequencePriorityQueue<P, T>::pop()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("Queue is empty!");
        }

        SequenceBlockType* bestBlock = this->findHighestPriorityBlock();
        T result = bestBlock->data_.data_;
        SequenceBlockType* lastBlock = this->getSequence()->accessLast();
        if (bestBlock != lastBlock)
        {
            using std::swap;
            swap(bestBlock->data_, lastBlock->data_);
        }
        this->getSequence()->removeLast();
        return result;
    }

    template<typename P, typename T>
    void UnsortedExplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        PQItem<P, T>& queueData = this->getSequence()->insertFirst().data_;
        queueData.priority_ = priority;
        queueData.data_ = data;
    }

    template<typename P, typename T>
    T UnsortedExplicitSequencePriorityQueue<P, T>::pop()
    {
        if (this->isEmpty())
        {
            throw std::out_of_range("Queue is empty!");
        }

        SequenceBlockType* bestBlock = this->findHighestPriorityBlock();
        T result = bestBlock->data_.data_;
        SequenceBlockType* firstBlock = this->getSequence()->accessFirst();
        if (bestBlock != firstBlock)
        {
            using std::swap;
            swap(bestBlock->data_, firstBlock->data_);
        }
        this->getSequence()->removeFirst();
        return result;
    }

    template<typename P, typename T>
    void SortedImplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        PQItem<P, T>* queueData = nullptr;

        if (this->isEmpty() || priority <= this->getSequence()->accessLast()->data_.priority_)
        {
            queueData = &this->getSequence()->insertLast().data_;
        }
        else if (priority >= this->getSequence()->accessFirst()->data_.priority_)
        {
            queueData = &this->getSequence()->insertFirst().data_;
        }
        else
        {
            queueData = &this->getSequence()->insertBefore(*this->getSequence()->findBlockWithProperty(
                [&priority](SequenceBlockType* block)->bool {
                    return block->data_.priority_ <= priority;
                })).data_;
        }

        queueData->priority_ = priority;
        queueData->data_ = data;
    }

    template<typename P, typename T>
    size_t SortedImplicitSequencePriorityQueue<P, T>::indexOfHighestPriorityBlock() const
    {
        return this->size() - 1;
    }

    template<typename P, typename T>
    void SortedExplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        PQItem<P, T>* queueData = nullptr;

        if (this->isEmpty() || priority <= this->getSequence()->accessFirst()->data_.priority_)
        {
            queueData = &this->getSequence()->insertFirst().data_;
        }
        else if (priority >= this->getSequence()->accessLast()->data_.priority_)
        {
            queueData = &this->getSequence()->insertLast().data_;
        }
        else
        {
            SequenceBlockType* prevBlock = this->getSequence()->findPreviousToBlockWithProperty(
                [priority](SequenceBlockType* block) -> bool
                {
                    return block->data_.priority_ >= priority;
                });
            queueData = &this->getSequence()->insertAfter(*prevBlock).data_;
        }

        queueData->priority_ = priority;
        queueData->data_ = data;
    }

    template<typename P, typename T>
    size_t SortedExplicitSequencePriorityQueue<P, T>::indexOfHighestPriorityBlock() const
    {
        return 0;
    }

    template<typename P, typename T>
    TwoLists<P, T>::TwoLists(size_t expectedSize):
        shortSequence_(new ShortSequenceType(std::ceil(std::sqrt(expectedSize)), false)),
        longSequence_(new LongSequenceType())
    {
    }

    template <typename P, typename T>
    TwoLists<P, T>::TwoLists(const TwoLists<P, T>& other) :
        shortSequence_(new ShortSequenceType()),
        longSequence_(new LongSequenceType())
    {
        assign(other);
    }

    template<typename P, typename T>
    TwoLists<P, T>::~TwoLists()
    {
        delete shortSequence_;
        shortSequence_ = nullptr;
        delete longSequence_;
        longSequence_ = nullptr;
    }

    template<typename P, typename T>
    ADT& TwoLists<P, T>::assign(const ADT& other)
    {
        if (this != &other) {
            const TwoLists<P, T>& otherTwoLists = dynamic_cast<const TwoLists<P, T>&>(other);

            shortSequence_->assign(*otherTwoLists.shortSequence_);
            longSequence_->assign(*otherTwoLists.longSequence_);
        }

        return *this;
    }

    template<typename P, typename T>
    void TwoLists<P, T>::clear()
    {
        shortSequence_->clear();
        longSequence_->clear();
    }

    template<typename P, typename T>
    size_t TwoLists<P, T>::size() const
    {
        return shortSequence_->size() + longSequence_->size();
    }

    template<typename P, typename T>
    bool TwoLists<P, T>::isEmpty() const
    {
        return shortSequence_->isEmpty();
    }

    template<typename P, typename T>
    bool TwoLists<P, T>::equals(const ADT& other)
    {
        if (this != &other) {
            if (this->size() != other.size()) { return false; }

            const TwoLists<P, T>& otherTwoLists = dynamic_cast<const TwoLists<P, T>&>(other);

            return shortSequence_->equals(*otherTwoLists.shortSequence_) && longSequence_->equals(*otherTwoLists.longSequence_);
        }
        else
        {
            return true;
        }
    }

    template<typename P, typename T>
    void TwoLists<P, T>::push(P priority, T data)
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    T& TwoLists<P, T>::peek()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    T TwoLists<P, T>::pop()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    BinaryHeap<P, T>::BinaryHeap() :
        ADS<PQItem<P, T>>(new amt::BinaryIH<PQItem<P, T>>())
    {
    }

    template<typename P, typename T>
    BinaryHeap<P, T>::BinaryHeap(const BinaryHeap& other) :
        ADS<PQItem<P, T>>(new amt::BinaryIH<PQItem<P, T>>(), other)
    {
    }

    template<typename P, typename T>
    void BinaryHeap<P, T>::push(P priority, T data)
    {
        PQItem<P, T>& queueData = this->getHierarchy()->insertLastLeaf().data_;
        queueData.priority_ = priority;
        queueData.data_ = data;

        HierarchyBlockType* currentBlock = this->getHierarchy()->accessLastLeaf();
        HierarchyBlockType* blockParent = this->getHierarchy()->accessParent(*currentBlock);

        while (blockParent != nullptr && currentBlock->data_.priority_ < blockParent->data_.priority_)
        {
            using std::swap;
            swap(currentBlock->data_, blockParent->data_);

            currentBlock = blockParent;
            blockParent = this->getHierarchy()->accessParent(*currentBlock);
        }
    }

    template<typename P, typename T>
    T& BinaryHeap<P, T>::peek()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    T BinaryHeap<P, T>::pop()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    amt::BinaryIH<PQItem<P, T>>* BinaryHeap<P, T>::getHierarchy()
    {
        return dynamic_cast<amt::BinaryIH<PQItem<P, T>>*>(this->memoryStructure_);
    }

}