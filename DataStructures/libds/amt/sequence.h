#pragma once

#include <libds/amt/abstract_memory_type.h>

namespace ds::amt {

	template<typename BlockType>
	class Sequence :
		virtual public AMT
	{
	public:
		virtual size_t calculateIndex(BlockType& block) = 0;

		virtual BlockType* accessFirst() const = 0;
		virtual BlockType* accessLast() const = 0;
		virtual BlockType* access(size_t index) const = 0;
		virtual BlockType* accessNext(const BlockType& block) const = 0;
		virtual BlockType* accessPrevious(const BlockType& block) const = 0;

		virtual BlockType& insertFirst() = 0;
		virtual BlockType& insertLast() = 0;
		virtual BlockType& insert(size_t index) = 0;
		virtual BlockType& insertAfter(BlockType& block) = 0;
		virtual BlockType& insertBefore(BlockType& block) = 0;

		virtual void removeFirst() = 0;
		virtual void removeLast() = 0;
		virtual void remove(size_t index) = 0;
		virtual void removeNext(const BlockType& block) = 0;
		virtual void removePrevious(const BlockType& block) = 0;

		virtual void processAllBlocksForward(std::function<void(BlockType*)> operation) const;
		virtual void processAllBlocksBackward(std::function<void(BlockType*)> operation) const;
		virtual void processBlocksForward(BlockType* block, std::function<void(BlockType*)> operation) const;
		virtual void processBlocksBackward(BlockType* block, std::function<void(BlockType*)> operation) const;
		BlockType* findBlockWithProperty(std::function<bool(BlockType*)> predicate) const;
		BlockType* findPreviousToBlockWithProperty(std::function<bool(BlockType*)> predicate) const;
	};

	template<typename BlockType>
    void Sequence<BlockType>::processAllBlocksForward(std::function<void(BlockType*)> operation) const
	{
		processBlocksForward(accessFirst(), operation);
	}

	template<typename BlockType>
    void Sequence<BlockType>::processAllBlocksBackward(std::function<void(BlockType*)> operation) const
	{
		processBlocksBackward(accessLast(), operation);
	}

	template<typename BlockType>
    void Sequence<BlockType>::processBlocksForward(BlockType* block, std::function<void(BlockType*)> operation) const
	{
		while (block != nullptr)
		{
			operation(block);
			block = accessNext(*block);
		}
	}

	template<typename BlockType>
    void Sequence<BlockType>::processBlocksBackward(BlockType* block, std::function<void(BlockType*)> operation) const
	{
		while (block != nullptr)
		{
			operation(block);
			block = accessPrevious(*block);
		}
	}

	template<typename BlockType>
    BlockType* Sequence<BlockType>::findBlockWithProperty(std::function<bool(BlockType*)> predicate) const
	{
		BlockType* block = accessFirst();

		while (block != nullptr && !predicate(block))
		{
			block = accessNext(*block);
		}

		return block;
	}

	template<typename BlockType>
    BlockType* Sequence<BlockType>::findPreviousToBlockWithProperty(std::function<bool(BlockType*)> predicate) const
	{
		BlockType* blockWithProperty = accessFirst();

		if (predicate(blockWithProperty))
		{
			return nullptr;
		}
		else
		{
			BlockType* prevToBlockWithProperty = blockWithProperty;
			blockWithProperty = accessNext(*blockWithProperty);

			while (blockWithProperty != nullptr && !predicate(blockWithProperty))
			{
				prevToBlockWithProperty = blockWithProperty;
				blockWithProperty = accessNext(*blockWithProperty);
			}

			return blockWithProperty != nullptr ? prevToBlockWithProperty : nullptr;
		}
	}

}