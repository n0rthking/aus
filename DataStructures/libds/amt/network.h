#pragma once

#include <libds/amt/abstract_memory_type.h>

namespace ds::amt {

	template<typename BlockType>
	class Network :
		virtual public AMT
	{
	public:
		using NodeType = BlockType;

		virtual size_t relationCount() const = 0;
		virtual size_t degree(const BlockType& node) const = 0;

		virtual BlockType* accessNodeFromGate(size_t order) const = 0;
		virtual BlockType* accessNodeFromNode(const BlockType& node, size_t order) const = 0;

		virtual bool relationExists(const BlockType& nodeA, const BlockType& nodeB) const = 0;

		virtual BlockType& insert() = 0;
		virtual void remove(BlockType* node) = 0;

		virtual void connect(BlockType& nodeA, BlockType& nodeB) = 0;
		virtual void disconnect(BlockType& nodeA, BlockType& nodeB) = 0;
	};

}