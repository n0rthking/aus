#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/network.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_sequence.h>
#include <functional>

namespace ds::amt {

	template<typename DataType>
	struct NetworkBlockImplicitRelations :
		public MemoryBlock<DataType>
	{
		using RelationBlockType = typename ImplicitAMS<NetworkBlockImplicitRelations<DataType>*>::BlockType;

		NetworkBlockImplicitRelations() : relations_(new IS<NetworkBlockImplicitRelations<DataType>*>()) {}
		~NetworkBlockImplicitRelations() { delete relations_; relations_ = nullptr; }

		IS<NetworkBlockImplicitRelations<DataType>*>* relations_;
	};

	template<typename DataType>
	using IRNetworkBlock = NetworkBlockImplicitRelations<DataType>;

	template<typename DataType>
	struct NetworkBlockExplicitRelations :
		public MemoryBlock<DataType>
	{
		using RelationBlockType = typename DoublyLS<NetworkBlockExplicitRelations<DataType>*>::BlockType;

		NetworkBlockExplicitRelations() : relations_(new DoublyLS<NetworkBlockExplicitRelations<DataType>*>()) {}
		~NetworkBlockExplicitRelations() { delete relations_; relations_ = nullptr; }

		DoublyLS<NetworkBlockExplicitRelations<DataType>*>* relations_;
	};

	template<typename DataType>
	using ERNetworkBlock = NetworkBlockExplicitRelations<DataType>;

	//----------

	template<typename BlockType, typename GateType>
	class ExplicitNetwork :
		public Network<BlockType>,
		public ExplicitAMS<BlockType>
	{
	public:
		using RelationsBlockType = typename BlockType::RelationBlockType;
		using GateBlockType = typename GateType::BlockType;
		using IteratorType = typename GateType::IteratorType;

		ExplicitNetwork() : gate_(new GateType()) {}
		~ExplicitNetwork() override { delete gate_; gate_ = nullptr; }

		AMT& assign(const AMT& other) override;
		void clear() override;
		size_t size() const override;
		bool equals(const AMT& other) override;

		size_t relationCount() const override;
		size_t degree(const BlockType& node) const override;

		BlockType* accessNodeFromGate(size_t order) const override;
		BlockType* accessNodeFromNode(const BlockType& node, size_t order) const override;

		bool relationExists(const BlockType& nodeA, const BlockType& nodeB) const override;

		BlockType& insert() override;
		void remove(BlockType* node) override;

		void connect(BlockType& nodeA, BlockType& nodeB) override;
		void disconnect(BlockType& nodeA, BlockType& nodeB) override;

		IteratorType begin();
		IteratorType end();

	protected:
		GateType* gate_;
	};

	template<typename DataType>
	class ImplicitGateImplicitRelationsNetwork :
		public ExplicitNetwork<IRNetworkBlock<DataType>, IS<IRNetworkBlock<DataType>*>>
	{
	};

	template<typename DataType>
	using IGIRNetwork = ImplicitGateImplicitRelationsNetwork<DataType>;

	template<typename DataType>
	class ImplicitGateExplicitRelationsNetwork :
		public ExplicitNetwork<ERNetworkBlock<DataType>, IS<ERNetworkBlock<DataType>*>>
	{
	};

	template<typename DataType>
	using IGERNetwork = ImplicitGateExplicitRelationsNetwork<DataType>;

	template<typename DataType>
	class ExplicitGateImplicitRelationsNetwork :
		public ExplicitNetwork<IRNetworkBlock<DataType>, DoublyLS<IRNetworkBlock<DataType>*>>
	{
	};

	template<typename DataType>
	using EGIRNetwork = ExplicitGateImplicitRelationsNetwork<DataType>;

	template<typename DataType>
	class ExplicitGateExplicitRelationsNetwork :
		public ExplicitNetwork<ERNetworkBlock<DataType>, DoublyLS<ERNetworkBlock<DataType>*>>
	{
	};

	template<typename DataType>
	using EGERNetwork = ExplicitGateExplicitRelationsNetwork<DataType>;

	//----------

	template<typename BlockType, typename GateType>
    AMT& ExplicitNetwork<BlockType, GateType>::assign(const AMT& other)
	{
		if (this != &other)
		{
			clear();

			const ExplicitNetwork<BlockType, GateType>& otherExplicitNetwork = dynamic_cast<const  ExplicitNetwork<BlockType, GateType>&>(other);

			otherExplicitNetwork.gate_->processAllBlocksForward([&](GateBlockType* b) { insert().data_ = b->data_->data_; });

			GateBlockType* myBlockFrom = gate_->accessFirst();
			GateBlockType* otherBlockFrom = otherExplicitNetwork.gate_->accessFirst();

			while (otherBlockFrom != nullptr)
			{
				otherBlockFrom->data_->relations_->processAllBlocksForward([&](RelationsBlockType* otherRelationsBlock)
					{
						size_t otherGateIndexTo = 0;
						otherExplicitNetwork.gate_->findBlockWithProperty([&](GateBlockType* otherGateBlockIndexTo)->bool
							{
								if (otherGateBlockIndexTo->data_ == otherRelationsBlock->data_)
								{
									return true;
								}
								else {
									otherGateIndexTo++;
									return false;
								}
							});
						GateBlockType* myBlockTo = gate_->access(otherGateIndexTo);
						connect(*myBlockFrom->data_, *myBlockTo->data_);
					});

				myBlockFrom = gate_->accessNext(*myBlockFrom);
				otherBlockFrom = otherExplicitNetwork.gate_->accessNext(*otherBlockFrom);
			}
		}
		return *this;
	}

	template<typename BlockType, typename GateType>
    void ExplicitNetwork<BlockType, GateType>::clear()
	{
		while (gate_->size() > 0)
		{
			AMS<BlockType>::memoryManager_->releaseMemory(gate_->accessLast()->data_);
			gate_->removeLast();
		}
	}

	template<typename BlockType, typename GateType>
    size_t ExplicitNetwork<BlockType, GateType>::size() const
	{
		return gate_->size();
	}

	template<typename BlockType, typename GateType>
    bool ExplicitNetwork<BlockType, GateType>::equals(const AMT& other)
	{
		if (this->size() != other.size()) { return false; }

		const ExplicitNetwork<BlockType, GateType>& otherExplicitNetwork = dynamic_cast<const ExplicitNetwork<BlockType, GateType>&>(other);

		GateBlockType* myGateBlock = gate_->accessFirst();
		GateBlockType* otherGateBlock = otherExplicitNetwork.gate_->accessFirst();

		while (myGateBlock != nullptr)
		{
			if (myGateBlock->data_ != otherGateBlock->data_) { return false; }
			if (myGateBlock->data_->relations_->size() != otherGateBlock->data_->relations_->size()) { return false; }

			RelationsBlockType* myRelationsBlock = myGateBlock->data_->relations_->accessFirst();
			RelationsBlockType* otherRelationsBlock = otherGateBlock->data_->relations_->accessFirst();

			while (myRelationsBlock != nullptr)
			{
				if (myRelationsBlock->data_ != otherRelationsBlock->data_) { return false; }

				myRelationsBlock = myGateBlock->data_->relations_->accessNext(*myRelationsBlock);
				otherRelationsBlock = otherGateBlock->data_->relations_->accessNext(*otherRelationsBlock);
			}

			myGateBlock = gate_->accessNext(*myGateBlock);
			otherGateBlock = otherExplicitNetwork.gate_->accessNext(*otherGateBlock);
		}

		return true;
	}

	template<typename BlockType, typename GateType>
    size_t ExplicitNetwork<BlockType, GateType>::relationCount() const
	{
		size_t result = 0;
		gate_->processAllBlocksForward([&result](const GateBlockType* b) {result += b->data_->relations_->size(); });
		return result;
	}

	template<typename BlockType, typename GateType>
    size_t ExplicitNetwork<BlockType, GateType>::degree(const BlockType& node) const
	{
		return node.relations_->size();
	}

	template<typename BlockType, typename GateType>
    BlockType* ExplicitNetwork<BlockType, GateType>::accessNodeFromGate(size_t order) const
	{
		return gate_->access(order)->data_;
	}

	template<typename BlockType, typename GateType>
    BlockType* ExplicitNetwork<BlockType, GateType>::accessNodeFromNode(const BlockType& node, size_t order) const
	{
		return node.relations_->access(order)->data_;
	}

	template<typename BlockType, typename GateType>
    bool ExplicitNetwork<BlockType, GateType>::relationExists(const BlockType& nodeA, const BlockType& nodeB) const
	{
		std::function<bool(const BlockType&, const BlockType&)> relationExists;
		relationExists = [](const BlockType& vrcholOd, const BlockType& vrcholDo)->bool
		{
			return vrcholOd.relations_->findBlockWithProperty([&vrcholDo](RelationsBlockType* b)->bool {return b->data_ == &vrcholDo; }) != nullptr;
		};

		return degree(nodeA) <= degree(nodeB) ? relationExists(nodeA, nodeB) : relationExists(nodeB, nodeA);
	}

	template<typename BlockType, typename GateType>
    BlockType& ExplicitNetwork<BlockType, GateType>::insert()
	{
		BlockType* newNode = AMS<BlockType>::memoryManager_->allocateMemory();
		gate_->insertLast().data_ = newNode;
		return *newNode;
	}

	template<typename BlockType, typename GateType>
    void ExplicitNetwork<BlockType, GateType>::remove(BlockType* node)
	{
		while (degree(*node) > 0)
		{
			disconnect(*node, *node->relations_->accessLast()->data_);
		}

		GateBlockType* gateBlock = gate_->accessFirst();
		if (gateBlock->data_ == node)
		{
			gate_->removeFirst();
		}
		else
		{
			GateBlockType* prevGateBlock = gate_->findPreviousToBlockWithProperty([&node](GateBlockType* b)->bool
				{
					return b->data_ == node;
				});
			gate_->removeNext(*prevGateBlock);
		}

		AMS<BlockType>::memoryManager_->releaseMemory(node);
	}

	template<typename BlockType, typename GateBlock>
    void ExplicitNetwork<BlockType, GateBlock>::connect(BlockType& nodeA, BlockType& nodeB)
	{
		nodeA.relations_->insertLast().data_ = &nodeB;
		nodeB.relations_->insertLast().data_ = &nodeA;
	}

	template<typename BlockType, typename GateBlock>
    void ExplicitNetwork<BlockType, GateBlock>::disconnect(BlockType& nodeA, BlockType& nodeB)
	{
		std::function<void(const BlockType&, const BlockType&)> disconnectRelation;
		disconnectRelation = [](const BlockType& nodeFrom, const BlockType& nodeTo)
		{

			if (nodeFrom.relations_->accessFirst()->data_ == &nodeTo)
			{
				nodeFrom.relations_->removeFirst();
			}
			else
			{
				RelationsBlockType* prevInRelationsFrom = nodeFrom.relations_->findPreviousToBlockWithProperty([&nodeTo](RelationsBlockType* b) -> bool
					{
						return b->data_ == &nodeTo;
					});
				nodeFrom.relations_->removeNext(*prevInRelationsFrom);
			}
		};

		disconnectRelation(nodeA, nodeB);
		disconnectRelation(nodeB, nodeA);
	}

	template<typename BlockType, typename GateType>
    typename ExplicitNetwork<BlockType, GateType>::IteratorType ExplicitNetwork<BlockType, GateType>::begin()
	{
		return gate_->begin();
	}

	template<typename BlockType, typename GateType>
    typename ExplicitNetwork<BlockType, GateType>::IteratorType ExplicitNetwork<BlockType, GateType>::end()
	{
		return gate_->end();
	}
}