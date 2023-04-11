#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/explicit_sequence.h>
#include <functional>

namespace ds::amt {

	template<typename BlockType>
	class Hierarchy :
		virtual public AMT
	{
	public:
		virtual size_t level(const BlockType& node) const;
		virtual size_t degree(const BlockType& node) const = 0;
		virtual size_t nodeCount() const;
	    virtual size_t nodeCount(const BlockType& node) const;

		virtual BlockType* accessRoot() const = 0;
		virtual BlockType* accessParent(const BlockType& node) const = 0;
		virtual BlockType* accessSon(const BlockType& node, size_t sonOrder) const = 0;

		virtual bool isRoot(const BlockType& node) const;
		virtual bool isNthSon(const BlockType& node, size_t sonOrder) const;
		virtual bool isLeaf(const BlockType& node) const;
		virtual bool hasNthSon(const BlockType& node, size_t sonOrder) const;

		virtual BlockType& emplaceRoot() = 0;
		virtual void changeRoot(BlockType* newRoot) = 0;

		virtual BlockType& emplaceSon(BlockType& parent, size_t sonOrder) = 0;
		virtual void changeSon(BlockType& parent, size_t sonOrder, BlockType* newSon) = 0;
		virtual void removeSon(BlockType& parent, size_t sonOrder) = 0;

		void processPreOrder(const BlockType* node, std::function<void(const BlockType*)> operation) const;
		void processPostOrder(BlockType* node, std::function<void(BlockType*)> operation) const;
		void processLevelOrder(BlockType* node, std::function<void(BlockType*)> operation) const;

	protected:
		using DataType = decltype(BlockType().data_);

		class DepthFirstIterator
		{
		protected:
			struct DepthFirstIteratorPosition
			{
				DepthFirstIteratorPosition(BlockType* currentNode, DepthFirstIteratorPosition* previousPosition) :
					currentNode_(currentNode),
					currentSon_(nullptr),
					currentSonOrder_(INVALID_INDEX),
					visitedSonCount_(0),
					currentNodeProcessed_(false),
					previousPosition_(previousPosition)
				{}

			    DepthFirstIteratorPosition(const DepthFirstIteratorPosition& other) :
					currentNode_(other.currentNode_),
					currentSon_(other.currentSon_),
					currentSonOrder_(other.currentSonOrder_),
					visitedSonCount_(other.visitedSonCount_),
					currentNodeProcessed_(other.currentNodeProcessed_),
					previousPosition_(other.previousPosition_)
				{}

			    ~DepthFirstIteratorPosition() {
					currentNode_ = nullptr;
					currentSon_ = nullptr;
					currentSonOrder_ = 0;
					visitedSonCount_ = 0;
					currentNodeProcessed_ = false;
					previousPosition_ = nullptr;
				}

				BlockType* currentNode_;
				BlockType* currentSon_;
				size_t currentSonOrder_;
				size_t visitedSonCount_;
				bool currentNodeProcessed_;
				DepthFirstIteratorPosition* previousPosition_;
			};

		public:
			DepthFirstIterator(Hierarchy<BlockType>* hierarchy);
			DepthFirstIterator(const DepthFirstIterator& other);
			~DepthFirstIterator();
			bool operator==(const DepthFirstIterator& other) const;
			bool operator!=(const DepthFirstIterator& other) const;
			DataType& operator*();
			BlockType& allData();

		protected:
			void savePosition(BlockType* currentNode);
			void removePosition();
			bool tryFindNextSonInCurrentPosition();

			Hierarchy<BlockType>* hierarchy_;
			DepthFirstIteratorPosition* currentPosition_;
		};

	public:
		class PreOrderHierarchyIterator :
			public DepthFirstIterator
		{
		public:
			PreOrderHierarchyIterator(Hierarchy<BlockType>* hierarchy, BlockType* node);
			PreOrderHierarchyIterator(const PreOrderHierarchyIterator& other);
			PreOrderHierarchyIterator& operator++();
			PreOrderHierarchyIterator& operator--();
			PreOrderHierarchyIterator& operator+=(size_t n);
		};

		//----------

		class PostOrderHierarchyIterator :
			public DepthFirstIterator
		{
		public:
			PostOrderHierarchyIterator(Hierarchy<BlockType>* hierarchy, BlockType* node);
			PostOrderHierarchyIterator(const PreOrderHierarchyIterator& other);
			PostOrderHierarchyIterator& operator++();
		};

		//----------

		PreOrderHierarchyIterator begin();
        PreOrderHierarchyIterator end();
		PreOrderHierarchyIterator beginPre();
		PreOrderHierarchyIterator endPre();
	    PostOrderHierarchyIterator beginPost();
        PostOrderHierarchyIterator endPost();
    };

	//----------

	template<typename BlockType, size_t K>
	class KWayHierarchy :
		virtual public Hierarchy<BlockType>
	{
	};

	//----------

	template<typename BlockType>
	class BinaryHierarchy :
		virtual public KWayHierarchy<BlockType, 2>
	{
	public:
		static const size_t LEFT_SON_INDEX = 0;
		static const size_t RIGHT_SON_INDEX = 1;

		BlockType* accessLeftSon(const BlockType& node) const;
        BlockType* accessRightSon(const BlockType& node) const;

        bool isLeftSon(const BlockType& node) const;
        bool isRightSon(const BlockType& node) const;

        bool hasLeftSon(const BlockType& node) const;
        bool hasRightSon(const BlockType& node) const;

        BlockType& insertLeftSon(BlockType& parent);
        BlockType& insertRightSon(BlockType& parent);

        void changeLeftSon(BlockType& parent, BlockType* newSon);
        void changeRightSon(BlockType& parent, BlockType* newSon);

        void removeLeftSon(BlockType& parent);
        void removeRightSon(BlockType& parent);

        void processInOrder(const BlockType* node, std::function<void(const BlockType*)> operation) const;

		//----------

		class InOrderHierarchyIterator :
			public Hierarchy<BlockType>::DepthFirstIterator
		{
		public:
			InOrderHierarchyIterator(BinaryHierarchy<BlockType>* hierarchy, BlockType* node);
			InOrderHierarchyIterator(const InOrderHierarchyIterator& other);
			InOrderHierarchyIterator& operator++();

		protected:
			bool tryToGoToLeftSonInCurrentPosition();
			bool tryToGoToRightSonInCurrentPosition();
		};

		InOrderHierarchyIterator begin();
        InOrderHierarchyIterator end();
    };

	//----------

	template<typename BlockType>
    size_t Hierarchy<BlockType>::level(const BlockType& node) const
	{
		size_t level = 0;
		BlockType* parent = accessParent(node);
		while (parent != nullptr)
		{
			level++;
			parent = accessParent(*parent);
		}
		return level;
	}

	template<typename BlockType>
    size_t Hierarchy<BlockType>::nodeCount() const
	{
		size_t result = 0;
		this->processPreOrder(this->accessRoot(), [&result](const BlockType* b)
			{
				result++;
			});
		return result;
	}

	template<typename BlockType>
    size_t Hierarchy<BlockType>::nodeCount(const BlockType& node) const
	{
		size_t result = 0;
		this->processPreOrder(&node, [&result](const BlockType* b)
			{
				result++;
			});
		return result;
	}

	template<typename BlockType>
    bool Hierarchy<BlockType>::isRoot(const BlockType& node) const
	{
		return this->accessParent(node) == nullptr;
	}

	template<typename BlockType>
    bool Hierarchy<BlockType>::isNthSon(const BlockType& node, size_t sonOrder) const
	{
		BlockType* parent = this->accessParent(node);
		return parent != nullptr && this->accessSon(*parent, sonOrder) == &node;
	}

	template<typename BlockType>
    bool Hierarchy<BlockType>::isLeaf(const BlockType& node) const
	{
		return this->degree(node) == 0;
	}

	template<typename BlockType>
    bool Hierarchy<BlockType>::hasNthSon(const BlockType& node, size_t sonOrder) const
	{
		return this->accessSon(node, sonOrder) != nullptr;
	}

	template<typename BlockType>
    void Hierarchy<BlockType>::processPreOrder(const BlockType* node, std::function<void(const BlockType*)> operation) const
	{
		if (node != nullptr)
		{
			operation(node);

		    size_t deg = this->degree(*node);
			size_t sonsProcessed = 0;
			size_t n = 0;
			while (sonsProcessed < deg)
			{
				BlockType* son = this->accessSon(*node, n);
				if (son != nullptr)
				{
					this->processPreOrder(son, operation);
					++sonsProcessed;
				}
				++n;
			}
		}
	}

	template<typename BlockType>
    void Hierarchy<BlockType>::processPostOrder(BlockType* node, std::function<void(BlockType*)> operation) const
	{
		if (node != nullptr)
		{
			size_t deg = this->degree(*node);
			size_t sonsProcessed = 0;
			size_t n = 0;
			while (sonsProcessed < deg)
			{
				BlockType* son = this->accessSon(*node, n);
				if (son != nullptr)
				{
					this->processPostOrder(son, operation);
					++sonsProcessed;
				}
				++n;
			}

			operation(node);
		}
	}

	template<typename BlockType>
    void Hierarchy<BlockType>::processLevelOrder(BlockType* node, std::function<void(BlockType*)> operation) const
	{
		if (node != nullptr)
		{
			SinglyLS<BlockType*> sequence;
			sequence.insertFirst().data_ = node;
			while (!sequence.isEmpty())
			{
				BlockType* current = sequence.accessFirst()->data_;
				sequence.removeFirst();
				if (current != nullptr)
				{
					operation(current);
					size_t nodeDegree = this->degree(*current);
					size_t n = 0;
					size_t sonsProcessed = 0;
					while (sonsProcessed < nodeDegree)
					{
						BlockType* son = this->accessSon(*current, n);
						if (son != nullptr)
						{
							sequence.insertLast().data_ = son;
							++sonsProcessed;
						}
						++n;
					}
				}
			}
		}
	}

	template<typename BlockType>
    void BinaryHierarchy<BlockType>::processInOrder(const BlockType* node, std::function<void(const BlockType*)> operation) const
	{
		if (node != nullptr)
		{
			this->processInOrder(this->accessLeftSon(*node), operation);
			operation(node);
			this->processInOrder(this->accessRightSon(*node), operation);
		}
	}

	template<typename BlockType>
    Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(Hierarchy<BlockType>* hierarchy) :
		hierarchy_(hierarchy),
		currentPosition_(nullptr)
	{
	}

	template<typename BlockType>
    Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(const DepthFirstIterator& other):
		DepthFirstIterator(other.hierarchy_)
	{
		DepthFirstIteratorPosition* myPosition = nullptr;
		for (DepthFirstIteratorPosition* otherPosition = other.currentPosition_;
			 otherPosition != nullptr;
			 otherPosition = otherPosition->previousPosition_)
		{
			if (currentPosition_ == nullptr)
			{
				currentPosition_ = new DepthFirstIteratorPosition(*otherPosition);
				myPosition = currentPosition_;
			}
			else
			{
				myPosition->previousPosition_ = new DepthFirstIteratorPosition(*otherPosition);
				myPosition = myPosition->previousPosition_;
			}
		}
	}

	template<typename BlockType>
    Hierarchy<BlockType>::DepthFirstIterator::~DepthFirstIterator()
	{
		while (currentPosition_ != nullptr)
		{
			this->removePosition();
		}

		hierarchy_ = nullptr;
		currentPosition_ = nullptr;
	}

	template<typename BlockType>
    bool Hierarchy<BlockType>::DepthFirstIterator::operator==(const DepthFirstIterator& other) const
	{
		if (hierarchy_ != other.hierarchy_) { return false; }

		DepthFirstIteratorPosition* myPosition = currentPosition_;
		DepthFirstIteratorPosition* otherPosition = other.currentPosition_;

		if (myPosition != nullptr && otherPosition != nullptr)
		{
			if (myPosition->currentNode_ != otherPosition->currentNode_ || myPosition->currentSonOrder_ != otherPosition->currentSonOrder_)
			{
				return false;
			}
		}

		return myPosition == nullptr && otherPosition == nullptr;
	}

	template<typename BlockType>
    bool Hierarchy<BlockType>::DepthFirstIterator::operator!=(const DepthFirstIterator& other) const
	{
		return ! (*this == other);
	}

	template<typename BlockType>
    auto Hierarchy<BlockType>::DepthFirstIterator::operator*() -> DataType&
	{
		currentPosition_->currentNodeProcessed_ = true;
		return currentPosition_->currentNode_->data_;
	}

	template<typename BlockType>
	auto Hierarchy<BlockType>::DepthFirstIterator::allData() -> BlockType&
	{
		currentPosition_->currentNodeProcessed_ = true;
		return *currentPosition_->currentNode_;
	}

	template<typename BlockType>
    void Hierarchy<BlockType>::DepthFirstIterator::savePosition(BlockType* currentNode)
	{
		currentPosition_ = new DepthFirstIteratorPosition(currentNode, currentPosition_);
	}

	template<typename BlockType>
    void Hierarchy<BlockType>::DepthFirstIterator::removePosition()
	{
		DepthFirstIteratorPosition* positionToRemove = currentPosition_;
		currentPosition_ = currentPosition_->previousPosition_;
		delete positionToRemove;
	}

	template<typename BlockType>
    bool Hierarchy<BlockType>::DepthFirstIterator::tryFindNextSonInCurrentPosition()
	{
		++currentPosition_->visitedSonCount_;

		size_t currentDegree = hierarchy_->degree(*currentPosition_->currentNode_);
		if (currentPosition_->visitedSonCount_ <= currentDegree)
		{
			do
			{
				++currentPosition_->currentSonOrder_;
				currentPosition_->currentSon_ = hierarchy_->accessSon(*currentPosition_->currentNode_, currentPosition_->currentSonOrder_);
			} while (currentPosition_->currentSon_ == nullptr);
			return true;
		}
		else
		{
			currentPosition_->currentSonOrder_ = INVALID_INDEX;
			currentPosition_->currentSon_ = nullptr;
			return false;
		}
	}

	template<typename BlockType>
    Hierarchy<BlockType>::PreOrderHierarchyIterator::PreOrderHierarchyIterator(Hierarchy<BlockType>* hierarchy, BlockType* node):
		Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(hierarchy)
	{
		if (node != nullptr)
		{
			this->savePosition(node);
		}
	}

	template<typename BlockType>
    Hierarchy<BlockType>::PreOrderHierarchyIterator::PreOrderHierarchyIterator(const PreOrderHierarchyIterator& other):
		Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(other)
	{
	}

	template<typename BlockType>
    typename Hierarchy<BlockType>::PreOrderHierarchyIterator& Hierarchy<BlockType>::PreOrderHierarchyIterator::operator++()
	{
		if (this->tryFindNextSonInCurrentPosition()) {
			this->savePosition(this->currentPosition_->currentSon_);
		}
		else {
			this->removePosition();
			if (this->currentPosition_ != nullptr) {
				++(*this);
			}
		}
		return *this;
	}

	template<typename BlockType>
	typename Hierarchy<BlockType>::PreOrderHierarchyIterator& Hierarchy<BlockType>::PreOrderHierarchyIterator::operator+=(size_t n)
	{
		auto newSon = this->hierarchy_->accessSon(*this->currentPosition_->currentNode_, n);
		if (newSon != nullptr) {
			this->savePosition(newSon);
			this->currentPosition_->previousPosition_->currentSonOrder_ = n;
			this->currentPosition_->previousPosition_->currentSon_ = newSon;
			this->currentPosition_->previousPosition_->visitedSonCount_ = n + 1;
			this->currentPosition_->previousPosition_->currentNodeProcessed_ = true;
		}
		return *this;
	}

	template<typename BlockType>
	typename Hierarchy<BlockType>::PreOrderHierarchyIterator& Hierarchy<BlockType>::PreOrderHierarchyIterator::operator--()
	{
		auto oldPosition = this->currentPosition_;
		auto newPosition = this->currentPosition_->previousPosition_;
		if (newPosition != nullptr) {
			this->currentPosition_ = newPosition;
			this->currentPosition_->visitedSonCount_ = 0;
			this->currentPosition_->currentSonOrder_ = INVALID_INDEX;
			this->currentPosition_->currentNodeProcessed_ = false;
			delete oldPosition;
		}
		return *this;
	}

	template<typename BlockType>
    Hierarchy<BlockType>::PostOrderHierarchyIterator::PostOrderHierarchyIterator(Hierarchy<BlockType>* hierarchy, BlockType* node) :
		Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(hierarchy)
	{
		if (node != nullptr)
		{
			this->savePosition(node);
			++(*this);
		}
	}

	template<typename BlockType>
    Hierarchy<BlockType>::PostOrderHierarchyIterator::PostOrderHierarchyIterator(const PreOrderHierarchyIterator& other) :
		Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(other)
	{
	}

	template<typename BlockType>
    typename Hierarchy<BlockType>::PostOrderHierarchyIterator& Hierarchy<BlockType>::PostOrderHierarchyIterator::operator++()
	{
		// TODO
		throw std::runtime_error("Not implemented yet!");
	}

    template <typename BlockType>
    typename Hierarchy<BlockType>::PreOrderHierarchyIterator Hierarchy<BlockType>::begin()
	{
	    return PreOrderHierarchyIterator(this, this->accessRoot());
	}

    template <typename BlockType>
    typename Hierarchy<BlockType>::PreOrderHierarchyIterator Hierarchy<BlockType>::end()
	{
	    return PreOrderHierarchyIterator(this, nullptr);
	}

    template <typename BlockType>
    typename Hierarchy<BlockType>::PreOrderHierarchyIterator Hierarchy<BlockType>::beginPre()
    {
		return PreOrderHierarchyIterator(this, this->accessRoot());
    }

    template <typename BlockType>
    typename Hierarchy<BlockType>::PreOrderHierarchyIterator Hierarchy<BlockType>::endPre()
    {
		return PreOrderHierarchyIterator(this, nullptr);
    }

    template <typename BlockType>
    typename Hierarchy<BlockType>::PostOrderHierarchyIterator Hierarchy<BlockType>::beginPost()
    {
        return PostOrderHierarchyIterator(this, this->accessRoot());
    }

    template <typename BlockType>
    typename Hierarchy<BlockType>::PostOrderHierarchyIterator Hierarchy<BlockType>::endPost()
    {
        return PostOrderHierarchyIterator(this, nullptr);
    }

    template <typename BlockType>
    BlockType* BinaryHierarchy<BlockType>::accessLeftSon(const BlockType& node) const
	{
	    return this->accessSon(node, LEFT_SON_INDEX);
	}

    template <typename BlockType>
    BlockType* BinaryHierarchy<BlockType>::accessRightSon(const BlockType& node) const
	{
	    return this->accessSon(node, RIGHT_SON_INDEX);
	}

    template <typename BlockType>
    bool BinaryHierarchy<BlockType>::isLeftSon(const BlockType& node) const
	{
	    return this->isNthSon(node, LEFT_SON_INDEX);
	}

    template <typename BlockType>
    bool BinaryHierarchy<BlockType>::isRightSon(const BlockType& node) const
	{
	    return this->isNthSon(node, RIGHT_SON_INDEX);
	}

    template <typename BlockType>
    bool BinaryHierarchy<BlockType>::hasLeftSon(const BlockType& node) const
	{
	    return this->hasNthSon(node, LEFT_SON_INDEX);
	}

    template <typename BlockType>
    bool BinaryHierarchy<BlockType>::hasRightSon(const BlockType& node) const
	{
	    return this->hasNthSon(node, RIGHT_SON_INDEX);
	}

    template <typename BlockType>
    BlockType& BinaryHierarchy<BlockType>::insertLeftSon(BlockType& parent)
	{
	    return this->emplaceSon(parent, LEFT_SON_INDEX);
	}

    template <typename BlockType>
    BlockType& BinaryHierarchy<BlockType>::insertRightSon(BlockType& parent)
	{
	    return this->emplaceSon(parent, RIGHT_SON_INDEX);
	}

    template <typename BlockType>
    void BinaryHierarchy<BlockType>::changeLeftSon(BlockType& parent, BlockType* newSon)
	{
	    this->changeSon(parent, LEFT_SON_INDEX, newSon);
	}

    template <typename BlockType>
    void BinaryHierarchy<BlockType>::changeRightSon(BlockType& parent, BlockType* newSon)
	{
	    this->changeSon(parent, RIGHT_SON_INDEX, newSon);
	}

    template <typename BlockType>
    void BinaryHierarchy<BlockType>::removeLeftSon(BlockType& parent)
	{
	    this->removeSon(parent, LEFT_SON_INDEX);
	}

    template <typename BlockType>
    void BinaryHierarchy<BlockType>::removeRightSon(BlockType& parent)
	{
	    this->removeSon(parent, RIGHT_SON_INDEX);
	}

    template<typename BlockType>
    BinaryHierarchy<BlockType>::InOrderHierarchyIterator::InOrderHierarchyIterator(BinaryHierarchy<BlockType>* hierarchy, BlockType* node):
		Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(hierarchy)
	{
		if (node != nullptr)
		{
			this->savePosition(node);
			++(*this);
		}
	}

	template<typename BlockType>
    BinaryHierarchy<BlockType>::InOrderHierarchyIterator::InOrderHierarchyIterator(const InOrderHierarchyIterator& other):
		Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(other)
	{
	}

	template<typename BlockType>
    typename BinaryHierarchy<BlockType>::InOrderHierarchyIterator& BinaryHierarchy<BlockType>::InOrderHierarchyIterator::operator++()
	{
		// TODO
		throw std::runtime_error("Not implemented yet!");
	}

	template<typename BlockType>
    bool BinaryHierarchy<BlockType>::InOrderHierarchyIterator::tryToGoToLeftSonInCurrentPosition()
	{
		this->currentPosition_->currentSon_ = this->hierarchy_->accessSon(*this->currentPosition_->currentNode_, BinaryHierarchy<BlockType>::LEFT_SON_INDEX);
		if (this->currentPosition_->currentSon_ != nullptr)
		{
			this->currentPosition_->currentSonOrder_ = BinaryHierarchy<BlockType>::LEFT_SON_INDEX;
			return true;
		}
		else
		{
			this->currentPosition_->currentSonOrder_ = INVALID_INDEX;
			return false;
		}
	}

	template<typename BlockType>
    bool BinaryHierarchy<BlockType>::InOrderHierarchyIterator::tryToGoToRightSonInCurrentPosition()
	{
		this->currentPosition_->currentSon_ = this->hierarchy_->accessSon(*this->currentPosition_->currentNode_, BinaryHierarchy<BlockType>::RIGHT_SON_INDEX);
		if (this->currentPosition_->currentSon_ != nullptr)
		{
			this->currentPosition_->currentSonOrder_ = BinaryHierarchy<BlockType>::RIGHT_SON_INDEX;
			return true;
		}
		else
		{
			this->currentPosition_->currentSonOrder_ = INVALID_INDEX;
			return false;
		}
	}

    template <typename BlockType>
    typename BinaryHierarchy<BlockType>::InOrderHierarchyIterator BinaryHierarchy<BlockType>::begin()
	{
	    return InOrderHierarchyIterator(this, this->accessRoot());
	}

    template <typename BlockType>
    typename BinaryHierarchy<BlockType>::InOrderHierarchyIterator BinaryHierarchy<BlockType>::end()
	{
	    return InOrderHierarchyIterator(this, nullptr);
	}
}
