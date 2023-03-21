#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/hierarchy.h>
#include <libds/amt/implicit_sequence.h>
#include <functional>

namespace ds::amt {

	template<typename DataType>
	struct ExplicitHierarchyBlock :
		public MemoryBlock<DataType>
	{
		ExplicitHierarchyBlock() : parent_(nullptr) {}
		~ExplicitHierarchyBlock() { parent_ = nullptr; }

		ExplicitHierarchyBlock<DataType>* parent_;
	};

	template<typename DataType>
	using EHBlock = ExplicitHierarchyBlock<DataType>;

	template<typename BlockType>
	class ExplicitHierarchy :
		virtual public Hierarchy<BlockType>,
		public ExplicitAMS<BlockType>
	{
	public:
		ExplicitHierarchy();
		ExplicitHierarchy(const ExplicitHierarchy& other);

		AMT& assign(const AMT& other) override;
		void clear() override;
		size_t size() const override;
		bool isEmpty() const override;
		bool equals(const AMT& other) override;

		BlockType* accessRoot() const override;
		BlockType* accessParent(const BlockType& node) const override;

		BlockType& emplaceRoot() override;
		void changeRoot(BlockType* newRoot) override;

	protected:
		BlockType* root_;
	};

	template<typename BlockType>
	using EH = ExplicitHierarchy<BlockType>;

	//----------

	template<typename DataType>
	struct MultiWayExplicitHierarchyBlock :
		public ExplicitHierarchyBlock<DataType>
	{
		MultiWayExplicitHierarchyBlock() : sons_(new IS<MultiWayExplicitHierarchyBlock<DataType>*>()) {}
		~MultiWayExplicitHierarchyBlock() { delete sons_; sons_ = nullptr; }

		IS<MultiWayExplicitHierarchyBlock<DataType>*>* sons_;
	};

	template<typename DataType>
	using MWEHBlock = MultiWayExplicitHierarchyBlock<DataType>;

	template<typename DataType>
	class MultiWayExplicitHierarchy :
		public ExplicitHierarchy<MultiWayExplicitHierarchyBlock<DataType>>
	{
	public:
		using BlockType = MultiWayExplicitHierarchyBlock<DataType>;

		MultiWayExplicitHierarchy();
		MultiWayExplicitHierarchy(const MultiWayExplicitHierarchy& other);
		~MultiWayExplicitHierarchy();

		size_t degree(const BlockType& node) const override;

		MultiWayExplicitHierarchyBlock<DataType>* accessSon(const BlockType& node, size_t sonOrder) const override;

		MultiWayExplicitHierarchyBlock<DataType>& emplaceSon(BlockType& parent, size_t sonOrder) override;
		void changeSon(BlockType& parent, size_t sonOrder, BlockType* newSon) override;
		void removeSon(BlockType& parent, size_t sonOrder) override;
	};

	template<typename DataType>
	using MultiWayEH = MultiWayExplicitHierarchy<DataType>;

	//----------

	template<typename DataType, size_t K>
	struct KWayExplicitHierarchyBlock :
		public ExplicitHierarchyBlock<DataType>
	{
		KWayExplicitHierarchyBlock() : sons_(new IS<KWayExplicitHierarchyBlock<DataType, K>*>(K, true)) {}
		~KWayExplicitHierarchyBlock() { delete sons_; sons_ = nullptr; }

		IS<KWayExplicitHierarchyBlock<DataType, K>*>* sons_;
	};

	template<typename DataType, size_t K>
	using KWEHBlock = KWayExplicitHierarchyBlock<DataType, K>;

	template<typename DataType, size_t K>
	class KWayExplicitHierarchy :
		public KWayHierarchy<KWayExplicitHierarchyBlock<DataType, K>, K>,
		public ExplicitHierarchy<KWayExplicitHierarchyBlock<DataType, K>>
	{
	public:
		using BlockType = KWayExplicitHierarchyBlock<DataType, K>;

		KWayExplicitHierarchy();
		KWayExplicitHierarchy(const KWayExplicitHierarchy& other);
		~KWayExplicitHierarchy();

		size_t degree(const BlockType& node) const override;

		KWayExplicitHierarchyBlock<DataType, K>* accessSon(const BlockType& node, size_t sonOrder) const override;

		KWayExplicitHierarchyBlock<DataType, K>& emplaceSon(BlockType& parent, size_t sonOrder) override;
		void changeSon(BlockType& parent, size_t sonOrder, BlockType* newSon) override;
		void removeSon(BlockType& parent, size_t sonOrder) override;
	};

	template<typename DataType, size_t K>
	using KWayEH = KWayExplicitHierarchy<DataType, K>;

	//----------

	template<typename DataType>
	struct BinaryExplicitHierarchyBlock :
		public ExplicitHierarchyBlock<DataType>
	{
		BinaryExplicitHierarchyBlock() : left_(nullptr), right_(nullptr) {}
		~BinaryExplicitHierarchyBlock() { left_ = nullptr; right_ = nullptr; }

		BinaryExplicitHierarchyBlock<DataType>* left_;
		BinaryExplicitHierarchyBlock<DataType>* right_;
	};

	template<typename DataType>
	using BEHBlock = BinaryExplicitHierarchyBlock<DataType>;

	template<typename DataType>
	class BinaryExplicitHierarchy :
		public BinaryHierarchy<BinaryExplicitHierarchyBlock<DataType>>,
		public ExplicitHierarchy<BinaryExplicitHierarchyBlock<DataType>>
	{
	public:
		using BlockType = BinaryExplicitHierarchyBlock<DataType>;

		BinaryExplicitHierarchy();
		BinaryExplicitHierarchy(const BinaryExplicitHierarchy& other);
		~BinaryExplicitHierarchy();

		size_t degree(const BlockType& node) const override;

		BinaryExplicitHierarchyBlock<DataType>* accessSon(const BlockType& node, size_t sonOrder) const override;

		BinaryExplicitHierarchyBlock<DataType>& emplaceSon(BlockType& parent, size_t sonOrder) override;
		void changeSon(BlockType& parent, size_t sonOrder, BlockType* newSon) override;
		void removeSon(BlockType& parent, size_t sonOrder) override;

		BlockType* accessLeftSon(const BlockType& node) const;
        BlockType* accessRightSon(const BlockType& node) const;

        bool isLeftSon(const BlockType& node) const;
        bool isRightSon(const BlockType& node) const;

        bool hasLeftSon(const BlockType& node) const;
        bool hasRightSon(const BlockType& node) const;

        BinaryExplicitHierarchyBlock<DataType>& insertLeftSon(BlockType& parent);
		BinaryExplicitHierarchyBlock<DataType>& insertRightSon(BlockType& parent);

		void changeLeftSon(BlockType& parent, BlockType* newSon);
		void changeRightSon(BlockType& parent, BlockType* newSon);

		void removeLeftSon(BlockType& parent);
		void removeRightSon(BlockType& parent);
	};

	template<typename DataType>
	using BinaryEH = BinaryExplicitHierarchy<DataType>;

	//----------

	template<typename BlockType>
    ExplicitHierarchy<BlockType>::ExplicitHierarchy() :
		root_(nullptr)
	{
	}

	template<typename BlockType>
    ExplicitHierarchy<BlockType>::ExplicitHierarchy(const ExplicitHierarchy& other) :
		ExplicitHierarchy()
	{
		this->assign(other);
	}

    template<typename BlockType>
    AMT& ExplicitHierarchy<BlockType>::assign(const AMT& other)
	{
		const ExplicitHierarchy<BlockType>& otherHierarchy = dynamic_cast<const ExplicitHierarchy<BlockType>&>(other);

		std::function<void(BlockType*, BlockType*)> copy;
		copy = [&](BlockType* myBlock, BlockType* otherBlock)
		{
		    myBlock->data_ = otherBlock->data_;

			size_t sonCount = otherHierarchy.degree(*otherBlock);
			size_t copiedSonCount = 0;
			size_t sonIndex = 0;
			while (copiedSonCount < sonCount)
			{
				BlockType* otherSon = otherHierarchy.accessSon(*otherBlock, sonIndex);
				if (otherSon != nullptr)
				{
					copy(&this->emplaceSon(*myBlock, sonIndex), otherSon);
					copiedSonCount++;
				}
				sonIndex++;
			}
		};

		clear();
		if (otherHierarchy.root_ != nullptr)
		{
			emplaceRoot();
			copy(root_, otherHierarchy.root_);
		}

		return *this;
	}

	template<typename BlockType>
    void ExplicitHierarchy<BlockType>::clear()
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename BlockType>
    size_t ExplicitHierarchy<BlockType>::size() const
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename BlockType>
    bool ExplicitHierarchy<BlockType>::isEmpty() const
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename BlockType>
    bool ExplicitHierarchy<BlockType>::equals(const AMT& other)
	{
		const ExplicitHierarchy<BlockType>& otherHierarchy = dynamic_cast<const ExplicitHierarchy<BlockType>&>(other);

		std::function<bool(BlockType*, BlockType*)> compare;
		compare = [&](BlockType* myBlock, BlockType* otherBlock) -> bool
		{
			if (myBlock == nullptr && otherBlock == nullptr) {
				return true;
			}
			else if (myBlock == nullptr || otherBlock == nullptr) {
				return false;
			}
			else if (this->degree(*myBlock) != otherHierarchy.degree(*otherBlock)) {
				return false;
			}
			else if (! (myBlock->data_ == otherBlock->data_)) {
				return false;
			}
			else {
				size_t maxDegree = this->degree(*myBlock);
				for (size_t i = 0; i < maxDegree; i++) {
					if (!compare(this->accessSon(*myBlock, i), otherHierarchy.accessSon(*otherBlock, i))) {
						return false;
					}
				}
				return true;
			}
		};

		return compare(root_, otherHierarchy.root_);
	}

	template<typename BlockType>
    BlockType* ExplicitHierarchy<BlockType>::accessRoot() const
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename BlockType>
    BlockType* ExplicitHierarchy<BlockType>::accessParent(const BlockType& node) const
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename BlockType>
    BlockType& ExplicitHierarchy<BlockType>::emplaceRoot()
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename BlockType>
    void ExplicitHierarchy<BlockType>::changeRoot(BlockType* newRoot)
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType>
	MultiWayExplicitHierarchy<DataType>::MultiWayExplicitHierarchy() :
		ExplicitHierarchy<MultiWayExplicitHierarchyBlock<DataType>>()
	{
	}

	template<typename DataType>
	MultiWayExplicitHierarchy<DataType>::MultiWayExplicitHierarchy(const MultiWayExplicitHierarchy& other) :
		ExplicitHierarchy<MultiWayExplicitHierarchyBlock<DataType>>()
	{
		this->assign(other);
	}

    template <typename DataType>
    MultiWayExplicitHierarchy<DataType>::~MultiWayExplicitHierarchy()
    {
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
    }

    template<typename DataType>
    size_t MultiWayExplicitHierarchy<DataType>::degree(const BlockType& node) const
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType>
    MultiWayExplicitHierarchyBlock<DataType>* MultiWayExplicitHierarchy<DataType>::accessSon(const BlockType& node, size_t sonOrder) const
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType>
    MultiWayExplicitHierarchyBlock<DataType>& MultiWayExplicitHierarchy<DataType>::emplaceSon(BlockType& parent, size_t sonOrder)
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType>
    void MultiWayExplicitHierarchy<DataType>::changeSon(BlockType& parent, size_t sonOrder, BlockType* newSon)
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType>
    void MultiWayExplicitHierarchy<DataType>::removeSon(BlockType& parent, size_t sonOrder)
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType, size_t K>
	KWayExplicitHierarchy<DataType, K>::KWayExplicitHierarchy() :
		ExplicitHierarchy<KWayExplicitHierarchyBlock<DataType, K>>()
	{
	}

	template<typename DataType, size_t K>
	KWayExplicitHierarchy<DataType, K>::KWayExplicitHierarchy(const KWayExplicitHierarchy& other) :
		ExplicitHierarchy<KWayExplicitHierarchyBlock<DataType, K>>()
	{
		this->assign(other);
	}

    template <typename DataType, size_t K>
    KWayExplicitHierarchy<DataType, K>::~KWayExplicitHierarchy()
    {
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
    }

    template<typename DataType, size_t K>
    size_t KWayExplicitHierarchy<DataType, K>::degree(const BlockType& node) const
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType, size_t K>
    KWayExplicitHierarchyBlock<DataType, K>* KWayExplicitHierarchy<DataType, K>::accessSon(const BlockType& node, size_t sonOrder) const
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType, size_t K>
    KWayExplicitHierarchyBlock<DataType, K>& KWayExplicitHierarchy<DataType, K>::emplaceSon(BlockType& parent, size_t sonOrder)
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType, size_t K>
    void KWayExplicitHierarchy<DataType, K>::changeSon(BlockType& parent, size_t sonOrder, BlockType* newSon)
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType, size_t K>
    void KWayExplicitHierarchy<DataType, K>::removeSon(BlockType& parent, size_t sonOrder)
	{
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename DataType>
	BinaryExplicitHierarchy<DataType>::BinaryExplicitHierarchy() :
		ExplicitHierarchy<BinaryExplicitHierarchyBlock<DataType>>()
	{
	}

	template<typename DataType>
	BinaryExplicitHierarchy<DataType>::BinaryExplicitHierarchy(const BinaryExplicitHierarchy& other) :
		ExplicitHierarchy<BinaryExplicitHierarchyBlock<DataType>>()
	{
		this->assign(other);
	}

    template <typename DataType>
    BinaryExplicitHierarchy<DataType>::~BinaryExplicitHierarchy()
    {
		// TODO 06
		// po implementacii vymazte vyhodenie vynimky!
		// throw std::runtime_error("Not implemented yet");
    }

    template<typename DataType>
    size_t BinaryExplicitHierarchy<DataType>::degree(const BlockType& node) const
	{
		size_t result = 0;
		if (node.left_ != nullptr) ++result;
		if (node.right_ != nullptr) ++result;
		return result;
	}

	template<typename DataType>
    BinaryExplicitHierarchyBlock<DataType>* BinaryExplicitHierarchy<DataType>::accessSon(const BlockType& node, size_t sonOrder) const
	{
		switch (sonOrder)
		{
		case BinaryHierarchy<BlockType>::LEFT_SON_INDEX:
			return node.left_;
		case BinaryHierarchy<BlockType>::RIGHT_SON_INDEX:
			return node.right_;
		default: return
			nullptr;
		}
	}

	template<typename DataType>
    BinaryExplicitHierarchyBlock<DataType>& BinaryExplicitHierarchy<DataType>::emplaceSon(BlockType& parent, size_t sonOrder)
	{
		if (sonOrder == BinaryHierarchy<BlockType>::LEFT_SON_INDEX)
		{
			return insertLeftSon(parent);
		}
		else
		{
			return insertRightSon(parent);
		}
	}

	template<typename DataType>
    void BinaryExplicitHierarchy<DataType>::changeSon(BlockType& parent, size_t sonOrder, BlockType* newSon)
	{
		if (sonOrder == BinaryHierarchy<BlockType>::LEFT_SON_INDEX)
		{
			changeLeftSon(parent, newSon);
		}
		else
		{
			changeRightSon(parent, newSon);
		}
	}

	template<typename DataType>
    void BinaryExplicitHierarchy<DataType>::removeSon(BlockType& parent, size_t sonOrder)
	{
		if (sonOrder == BinaryHierarchy<BlockType>::LEFT_SON_INDEX)
		{
			removeLeftSon(parent);
		}
		else
		{
			removeRightSon(parent);
		}
	}

    template <typename DataType>
    auto BinaryExplicitHierarchy<DataType>::accessLeftSon(const BlockType& node) const -> BlockType*
    {
	    return node.left_;
	}

    template <typename DataType>
    auto BinaryExplicitHierarchy<DataType>::accessRightSon(const BlockType& node) const -> BlockType*
	{
	    return node.right_;
	}

    template <typename DataType>
    bool BinaryExplicitHierarchy<DataType>::isLeftSon(const BlockType& node) const
	{
	    return node.parent_ != nullptr && static_cast<BlockType*>(node.parent_)->left_ == &node;
	}

    template <typename DataType>
    bool BinaryExplicitHierarchy<DataType>::isRightSon(const BlockType& node) const
	{
	    return node.parent_ != nullptr && static_cast<BlockType*>(node.parent_)->right_ == &node;
	}

    template <typename DataType>
    bool BinaryExplicitHierarchy<DataType>::hasLeftSon(const BlockType& node) const
	{
	    return node.left_ != nullptr;
	}

    template <typename DataType>
    bool BinaryExplicitHierarchy<DataType>::hasRightSon(const BlockType& node) const
	{
	    return node.right_ != nullptr;
	}

    template<typename DataType>
    BinaryExplicitHierarchyBlock<DataType>& BinaryExplicitHierarchy<DataType>::insertLeftSon(BlockType& parent)
	{
		BlockType* newSon = AbstractMemoryStructure<BlockType>::memoryManager_->allocateMemory();
		parent.left_ = newSon;
		newSon->parent_ = &parent;
		return *newSon;
	}

	template<typename DataType>
    BinaryExplicitHierarchyBlock<DataType>& BinaryExplicitHierarchy<DataType>::insertRightSon(BlockType& parent)
	{
		BlockType* newSon = AbstractMemoryStructure<BlockType>::memoryManager_->allocateMemory();
		parent.right_ = newSon;
		newSon->parent_ = &parent;
		return *newSon;
	}

	template<typename DataType>
    void BinaryExplicitHierarchy<DataType>::changeLeftSon(BlockType& parent, BlockType* newSon)
	{
		BlockType* oldSon = parent.left_;
		parent.left_ = newSon;
		if (oldSon != nullptr) { oldSon->parent_ = nullptr; }
		if (newSon != nullptr) { newSon->parent_ = &parent; }
	}

	template<typename DataType>
    void BinaryExplicitHierarchy<DataType>::changeRightSon(BlockType& parent, BlockType* newSon)
	{
		BlockType* oldSon = parent.right_;
		parent.right_ = newSon;
		if (oldSon != nullptr) { oldSon->parent_ = nullptr; }
		if (newSon != nullptr) { newSon->parent_ = &parent; }
	}

	template<typename DataType>
    void BinaryExplicitHierarchy<DataType>::removeLeftSon(BlockType& parent)
	{
		BlockType* removedSon = parent.left_;

		Hierarchy<BlockType>::processPostOrder(removedSon, [&](BlockType* b)
		{
		    AbstractMemoryStructure<BlockType>::memoryManager_->releaseMemory(b);
		});

		parent.left_ = nullptr;
	}

	template<typename DataType>
    void BinaryExplicitHierarchy<DataType>::removeRightSon(BlockType& parent)
	{
		BlockType* removedSon = parent.right_;

		Hierarchy<BlockType>::processPostOrder(removedSon, [&](BlockType* b)
		{
		    AbstractMemoryStructure<BlockType>::memoryManager_->releaseMemory(b);
		});

		parent.right_ = nullptr;
	}

}