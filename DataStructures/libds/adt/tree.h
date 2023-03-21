#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_hierarchy.h>
#include <libds/amt/explicit_hierarchy.h>

namespace ds::adt {

    template <typename T>
    class Tree :
        virtual public ADT
    {
    public:
        using Node = amt::MemoryBlock<T>;

        virtual size_t degree(const Node& node) const = 0;
        virtual size_t nodeCount() const = 0;
        virtual size_t nodeCount(const Node& node) const = 0;

        virtual Node* accessRoot() const = 0;
        virtual Node* accessParent(const Node& node) const = 0;
        virtual Node* accessSon(const Node& node, size_t sonOrder) const = 0;

        virtual Node& insertRoot() = 0;
        virtual void changeRoot(Node* newRoot) = 0;

        virtual Node& emplaceSon(Node& parent, size_t sonOrder) = 0;
        virtual void changeSon(Node& parent, size_t sonOrder, Node* newSon) = 0;
        virtual void removeSon(Node& parent, size_t sonOrder) = 0;

        virtual bool isRoot(const Node& node) const = 0;
        virtual bool isNThSon(const Node& node, size_t sonOrder) const = 0;
        virtual bool isLeaf(const Node& node) const = 0;
        virtual bool hasNThSon(const Node& node, size_t sonOrder) const = 0;
    };

    //----------

    template <typename T, typename HierarchyType>
    class GeneralTree :
        virtual public Tree<T>,
        public ADS<T>
    {
    public:
        using Node = typename Tree<T>::Node;
        using IteratorType = typename HierarchyType::IteratorType;

    public:
        GeneralTree();
        GeneralTree(const GeneralTree& other);

        size_t degree(const Node& node) const override;
        size_t nodeCount() const override;
        size_t nodeCount(const Node& node) const override;

        Node* accessRoot() const override;
        Node* accessParent(const Node& node) const override;
        Node* accessSon(const Node& node, size_t sonOrder) const override;

        Node& insertRoot() override;
        void changeRoot(Node* newRoot) override;

        Node& emplaceSon(Node& parent, size_t sonOrder) override;
        void changeSon(Node& parent, size_t sonOrder, Node* newSon) override;
        void removeSon(Node& parent, size_t sonOrder) override;

        bool isRoot(const Node& node) const override;
        bool isNThSon(const Node& node, size_t sonOrder) const override;
        bool isLeaf(const Node& node) const override;
        bool hasNThSon(const Node& node, size_t sonOrder) const override;

        IteratorType begin();
        IteratorType end();

    private:
        using NodeType = typename HierarchyType::BlockType;

        static NodeType* getNode(Node* node);
        static NodeType& getNode(Node& node);
        static const NodeType& getNode(const Node& node);
        HierarchyType* getHierarchy() const;
    };

    //----------

    template<typename T>
    class MultiwayTree : public GeneralTree<T, amt::MultiWayEH<T>>
    {
    };

    //----------

    template<typename T, size_t K>
    class ImplicitKWayTree : public GeneralTree<T, amt::ImplicitHierarchy<T, K>>
    {
    };

    //----------

    template<typename T, size_t K>
    class ExplicitKWayTree : public GeneralTree<T, amt::KWayEH<T, K>>
    {
    };

    //----------

    template<typename T>
    class ImplicitBinaryTree : public GeneralTree<T, amt::ImplicitHierarchy<T, 2>>
    {
    };

    //----------

    template<typename T>
    class ExplicitBinaryTree : public GeneralTree<T, amt::BinaryEH<T>>
    {
    };

    //----------

    template <typename T, typename HierarchyType>
    GeneralTree<T, HierarchyType>::GeneralTree() :
        ADS<T>(new HierarchyType())
    {
    }

    template <typename T, typename HierarchyType>
    GeneralTree<T, HierarchyType>::GeneralTree(const GeneralTree& other) :
        ADS<T>(new HierarchyType(), other)
    {
    }

    template <typename T, typename HierarchyType>
    size_t GeneralTree<T, HierarchyType>::degree(const Node& node) const
    {
        return this->getHierarchy()->degree(this->getNode(node));
    }

    template <typename T, typename HierarchyType>
    size_t GeneralTree<T, HierarchyType>::nodeCount() const
    {
        return this->getHierarchy()->nodeCount();
    }

    template <typename T, typename HierarchyType>
    size_t GeneralTree<T, HierarchyType>::nodeCount(const Node& node) const
    {
        return this->getHierarchy()->nodeCount(this->getNode(node));
    }

    template <typename T, typename HierarchyType>
    typename GeneralTree<T, HierarchyType>::Node* GeneralTree<T, HierarchyType>::accessRoot() const
    {
        return this->getHierarchy()->accessRoot();
    }

    template <typename T, typename HierarchyType>
    typename GeneralTree<T, HierarchyType>::Node* GeneralTree<T, HierarchyType>::accessParent(const Node& node) const
    {
        return this->getHierarchy()->accessParent(this->getNode(node));
    }

    template <typename T, typename HierarchyType>
    typename GeneralTree<T, HierarchyType>::Node* GeneralTree<T, HierarchyType>::accessSon(const Node& node, size_t sonOrder) const
    {
        typename HierarchyType::BlockType* son = this->getHierarchy()->accessSon(this->getNode(node), sonOrder);
        if (son == nullptr)
        {
            this->error("No such son!");
        }
        return son;
    }

    template <typename T, typename HierarchyType>
    typename GeneralTree<T, HierarchyType>::Node& GeneralTree<T, HierarchyType>::insertRoot()
    {
        return this->getHierarchy()->emplaceRoot();
    }

    template <typename T, typename HierarchyType>
    void GeneralTree<T, HierarchyType>::changeRoot(Node* newRoot)
    {
        this->getHierarchy()->changeRoot(this->getNode(newRoot));
    }

    template <typename T, typename HierarchyType>
    typename GeneralTree<T, HierarchyType>::Node& GeneralTree<T, HierarchyType>::emplaceSon(Node& parent, size_t sonORd)
    {
        return this->getHierarchy()->emplaceSon(this->getNode(parent), sonORd);
    }

    template <typename T, typename HierarchyType>
    void GeneralTree<T, HierarchyType>::changeSon(Node& parent, size_t sonOrder, Node* newSon)
    {
        this->getHierarchy()->changeSon(this->getNode(parent), sonOrder, this->getNode(newSon));
    }

    template <typename T, typename HierarchyType>
    void GeneralTree<T, HierarchyType>::removeSon(Node& parent, size_t sonOrder)
    {
        this->getHierarchy()->removeSon(this->getNode(parent), sonOrder);
    }

    template <typename T, typename HierarchyType>
    bool GeneralTree<T, HierarchyType>::isRoot(const Node& node) const
    {
        return this->getHierarchy()->isRoot(this->getNode(node));
    }

    template <typename T, typename HierarchyType>
    bool GeneralTree<T, HierarchyType>::isNThSon(const Node& node, size_t sonOrder) const
    {
        return this->getHierarchy()->isNthSon(this->getNode(node), sonOrder);
    }

    template <typename T, typename HierarchyType>
    bool GeneralTree<T, HierarchyType>::isLeaf(const Node& node) const
    {
        return this->getHierarchy()->isLeaf(this->getNode(node));
    }

    template <typename T, typename HierarchyType>
    bool GeneralTree<T, HierarchyType>::hasNThSon(const Node& node, size_t sonOrder) const
    {
        return this->getHierarchy()->hasNthSon(this->getNode(node), sonOrder);
    }

    template <typename T, typename HierarchyType>
    auto GeneralTree<T, HierarchyType>::begin() -> IteratorType
    {
        return this->getHierarchy()->begin();
    }

    template <typename T, typename HierarchyType>
    auto GeneralTree<T, HierarchyType>::end() -> IteratorType
    {
        return this->getHierarchy()->end();
    }

    template<typename T, typename HierarchyType>
    auto GeneralTree<T, HierarchyType>::getNode(Node* node) -> NodeType*
    {
        return static_cast<NodeType*>(node);
    }

    template<typename T, typename HierarchyType>
    auto GeneralTree<T, HierarchyType>::getNode(Node& node) -> NodeType&
    {
        return static_cast<NodeType&>(node);
    }

    template<typename T, typename HierarchyType>
    auto GeneralTree<T, HierarchyType>::getNode(const Node& node) -> const NodeType&
    {
        return static_cast<const NodeType&>(node);
    }

    template <typename T, typename HierarchyType>
    HierarchyType* GeneralTree<T, HierarchyType>::getHierarchy() const
    {
        return static_cast<HierarchyType*>(this->memoryStructure_);
    }
}
