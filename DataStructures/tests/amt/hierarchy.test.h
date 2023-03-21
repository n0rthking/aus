#pragma once

#include <tests/_details/test.hpp>
#include <libds/amt/implicit_hierarchy.h>
#include <libds/amt/explicit_hierarchy.h>
#include <memory>

namespace ds::tests
{
    namespace details
    {
        amt::MultiWayExplicitHierarchy<int> makeHierarchy()
        {
            amt::MultiWayExplicitHierarchy<int> hierarchy;
            auto& root = hierarchy.emplaceRoot();
            auto& one = hierarchy.emplaceSon(root, 0);
            auto& two = hierarchy.emplaceSon(root, 1);
            root.data_ = 0;
            one.data_ = 1;
            two.data_ = 2;
            hierarchy.emplaceSon(one, 0).data_ = 3;
            hierarchy.emplaceSon(one, 1).data_ = 4;
            hierarchy.emplaceSon(one, 2).data_ = 5;
            hierarchy.emplaceSon(two, 0).data_ = 6;
            return hierarchy;
        }

        amt::BinaryImplicitHierarchy<int> makeBinaryHierarchy()
        {
            amt::BinaryImplicitHierarchy<int> hierarchy;
            hierarchy.insertLastLeaf().data_ = 10;
            hierarchy.insertLastLeaf().data_ = 5;
            hierarchy.insertLastLeaf().data_ = 15;
            hierarchy.insertLastLeaf().data_ = 2;
            hierarchy.insertLastLeaf().data_ = 7;
            hierarchy.insertLastLeaf().data_ = 12;
            return hierarchy;
        }
    }

    /**
     *  @brief Tests pre-order traversal of a hierarchy.
     */
    class HierarchyTestProcessPreOrder : public LeafTest
    {
    public:
        HierarchyTestProcessPreOrder() :
            LeafTest("process-pre-order")
        {
        }

    protected:
        void test() override
        {
            //        0
            //   /         \
            //   1         2
            // / | \       |
            // 3 4 5       6
            const auto hierarchy = details::makeHierarchy();
            const auto order = { 0, 1, 3, 4, 5, 2, 6 };
            auto orderIt = begin(order);
            hierarchy.processPreOrder(hierarchy.accessRoot(), [&orderIt, this](auto* node)
                {
                    this->assert_equals(*orderIt, node->data_);
                    ++orderIt;
                });
        }
    };

    /**
     *  @brief Tests post-order traversal of a hierarchy.
     */
    class HierarchyTestProcessPostOrder : public LeafTest
    {
    public:
        HierarchyTestProcessPostOrder() :
            LeafTest("process-post-order")
        {
        }

    protected:
        void test() override
        {
            //        0
            //   /         \
            //   1         2
            // / | \       |
            // 3 4 5       6
            const auto hierarchy = details::makeHierarchy();
            const auto order = { 3, 4, 5, 1, 6, 2, 0 };
            auto orderIt = begin(order);
            hierarchy.processPostOrder(hierarchy.accessRoot(), [&orderIt, this](auto* node)
                {
                    this->assert_equals(*orderIt, node->data_);
                   ++orderIt;
                });
        }
    };

    /**
     *  @brief Tests level-order traversal of a hierarchy.
     */
    class HierarchyTestProcessLevelOrder : public LeafTest
    {
    public:
        HierarchyTestProcessLevelOrder() :
            LeafTest("process-level-order")
        {
        }

    protected:
        void test() override
        {
            //        0
            //   /         \
            //   1         2
            // / | \       |
            // 3 4 5       6
            const auto hierarchy = details::makeHierarchy();
            const auto order = { 0, 1, 2, 3, 4, 5, 6 };
            auto orderIt = begin(order);
            hierarchy.processLevelOrder(hierarchy.accessRoot(), [&orderIt, this](auto* node)
                {
                    this->assert_equals(*orderIt, node->data_);
                    ++orderIt;
                });
        }
    };

    /**
     *  @brief Tests pre-order iterator.
     */
    class HierarchyTestPreOrderIterator : public LeafTest
    {
    public:
        HierarchyTestPreOrderIterator() :
            LeafTest("pre-order-iterator")
        {
        }

    protected:
        void test() override
        {
            //        0
            //   /         \
            //   1         2
            // / | \       |
            // 3 4 5       6
            auto hierarchy = details::makeHierarchy();
            const auto order = { 0, 1, 3, 4, 5, 2, 6 };
            auto orderIt = begin(order);
            auto hierarchyIt = hierarchy.beginPre();
            const auto size = static_cast<int>(hierarchy.size());
            for (int i = 0; i < size; ++i)
            {
                this->assert_equals(*orderIt, *hierarchyIt);
                ++orderIt;
                ++hierarchyIt;
            }
            this->assert_equals(hierarchy.endPre(), hierarchyIt);
        }
    };

    /**
     *  @brief Tests post-order iterator.
     */
    class HierarchyTestPostOrderIterator : public LeafTest
    {
    public:
        HierarchyTestPostOrderIterator() :
            LeafTest("post-order-iterator")
        {
        }

    protected:
        void test() override
        {
            //        0
            //   /         \
            //   1         2
            // / | \       |
            // 3 4 5       6
            auto hierarchy = details::makeHierarchy();
            const auto order = { 3, 4, 5, 1, 6, 2, 0 };
            auto orderIt = begin(order);
            auto hierarchyIt = hierarchy.beginPost();
            const auto size = static_cast<int>(hierarchy.size());
            for (int i = 0; i < size; ++i)
            {
                this->assert_equals(*orderIt, *hierarchyIt);
                ++orderIt;
                ++hierarchyIt;
            }
            this->assert_equals(hierarchy.endPost(), hierarchyIt);
        }
    };

    /**
     *  @brief Tests in-order traversal of a binary hierarchy.
     */
    class BinaryHierarchyTestProcessInOrder : public LeafTest
    {
    public:
        BinaryHierarchyTestProcessInOrder() :
            LeafTest("process-in-order")
        {
        }

    protected:
        void test() override
        {
            //        10
            //   /          \
            //   5          15
            // /   \      /
            // 2   7     12
            const auto hierarchy = details::makeBinaryHierarchy();
            const auto order = { 2, 5, 7, 10, 12, 15 };
            auto orderIt = begin(order);
            hierarchy.processInOrder(hierarchy.accessRoot(), [&orderIt, this](auto* node)
                {
                    this->assert_equals(*orderIt, node->data_);
                    ++orderIt;
                });
        }
    };

    /**
     *  @brief Tests post-order iterator.
     */
    class BinaryHierarchyTestInOrderIterator : public LeafTest
    {
    public:
        BinaryHierarchyTestInOrderIterator() :
            LeafTest("in-order-iterator")
        {
        }

    protected:
        void test() override
        {
            //        10
            //   /          \
            //   5          15
            // /   \      /
            // 2   7     12
            auto hierarchy = details::makeBinaryHierarchy();
            const auto order = { 2, 5, 7, 10, 12, 15 };
            auto orderIt = begin(order);
            auto hierarchyIt = hierarchy.begin();
            const auto size = static_cast<int>(hierarchy.size());
            for (int i = 0; i < size; ++i)
            {
                this->assert_equals(*orderIt, *hierarchyIt);
                ++orderIt;
                ++hierarchyIt;
            }
            this->assert_equals(hierarchy.end(), hierarchyIt);
        }
    };

    /**
     *  @brief Test for processing elements in various orders.
     */
    class HierarchyTest : public CompositeTest
    {
    public:
        HierarchyTest() :
            CompositeTest("Hierarchy")
        {
            this->add_test(std::make_unique<HierarchyTestProcessPreOrder>());
            this->add_test(std::make_unique<HierarchyTestProcessPostOrder>());
            this->add_test(std::make_unique<HierarchyTestProcessLevelOrder>());
            this->add_test(std::make_unique<HierarchyTestPreOrderIterator>());
            this->add_test(std::make_unique<HierarchyTestPostOrderIterator>());
            this->add_test(std::make_unique<BinaryHierarchyTestProcessInOrder>());
            this->add_test(std::make_unique<BinaryHierarchyTestInOrderIterator>());
        }
    };
}
