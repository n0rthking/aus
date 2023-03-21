#pragma once

#include <libds/amt/explicit_hierarchy.h>
#include <tests/_details/test.hpp>
#include <memory>

namespace ds::tests
{
    /**
     * @brief Tests insertion of root and sons.
     */
    class MWEHTestInsert : public LeafTest
    {
    public:
        MWEHTestInsert() :
            LeafTest("insert")
        {
        }

    protected:
        void test() override
        {
            amt::MultiWayExplicitHierarchy<int> hierarchy;
            auto& root = hierarchy.emplaceRoot();
            auto& one = hierarchy.emplaceSon(root, 0);
            auto& two = hierarchy.emplaceSon(root, 1);
            hierarchy.emplaceSon(one, 0);
            hierarchy.emplaceSon(one, 1);
            hierarchy.emplaceSon(one, 2);
            hierarchy.emplaceSon(two, 0);
            //        0
            //   /         \
            //   1         2
            // / | \       |
            // 3 4 5       6

            this->assert_equals(static_cast<size_t>(7), hierarchy.size());
        }
    };

    /**
     *  @brief Tests access of parent and sons.
     */
    class MWEHTestAccess : public LeafTest
    {
    public:
        MWEHTestAccess() :
            LeafTest("access")
        {
        }

    protected:
        void test() override
        {
            amt::MultiWayExplicitHierarchy<int> hierarchy;
            this->assert_null(hierarchy.accessRoot());

            auto& root1 = hierarchy.emplaceRoot();
            auto& one1 = hierarchy.emplaceSon(root1, 0);
            auto& two1 = hierarchy.emplaceSon(root1, 1);
            root1.data_ = 0;
            one1.data_ = 1;
            two1.data_ = 2;
            hierarchy.emplaceSon(one1, 0).data_ = 3;
            hierarchy.emplaceSon(one1, 1).data_ = 4;
            hierarchy.emplaceSon(one1, 2).data_ = 5;
            hierarchy.emplaceSon(two1, 0).data_ = 6;
            //        0
            //   /         \
            //   1         2
            // / | \       |
            // 3 4 5       6

            auto* root = hierarchy.accessRoot();
            this->assert_not_null(root);
            this->assert_equals(0, root->data_);

            auto* one = hierarchy.accessSon(*root, 0);
            this->assert_not_null(one);
            this->assert_equals(1, one->data_);

            auto* two = hierarchy.accessSon(*root, 1);
            this->assert_not_null(two);
            this->assert_equals(2, two->data_);

            auto* six = hierarchy.accessSon(*two, 0);
            this->assert_not_null(six);
            this->assert_equals(6, six->data_);

            auto* notseven = hierarchy.accessSon(*two, 1);
            this->assert_null(notseven);

            this->assert_equals(two, hierarchy.accessParent(*six));
            this->assert_equals(root, hierarchy.accessParent(*one));
            this->assert_null(hierarchy.accessParent(*root));
        }
    };

    /**
     * @brief Tests method for calculation of node count, level and degree.
     */
    class MWEHTestLevelsCountsDegs : public LeafTest
    {
    public:
        MWEHTestLevelsCountsDegs() :
            LeafTest("level-count-degree")
        {
        }

    protected:
        void test() override
        {
            amt::MultiWayExplicitHierarchy<int> hierarchy;

            auto& root = hierarchy.emplaceRoot();
            auto& one = hierarchy.emplaceSon(root, 0);
            auto& two = hierarchy.emplaceSon(root, 1);
            hierarchy.emplaceSon(one, 0);
            hierarchy.emplaceSon(one, 1);
            hierarchy.emplaceSon(one, 2);
            auto& six = hierarchy.emplaceSon(two, 0);
            //        0
            //   /         \
            //   1         2
            // / | \       |
            // 3 4 5       6

            this->assert_equals(static_cast<size_t>(7), hierarchy.nodeCount());
            this->assert_equals(static_cast<size_t>(4), hierarchy.nodeCount(one));

            this->assert_equals(static_cast<size_t>(0), hierarchy.level(root));
            this->assert_equals(static_cast<size_t>(1), hierarchy.level(one));
            this->assert_equals(static_cast<size_t>(2), hierarchy.level(six));

            this->assert_equals(static_cast<size_t>(2), hierarchy.degree(root));
            this->assert_equals(static_cast<size_t>(3), hierarchy.degree(one));
            this->assert_equals(static_cast<size_t>(1), hierarchy.degree(two));
            this->assert_equals(static_cast<size_t>(0), hierarchy.degree(six));
        }
    };

    /**
     *  @brief Tests removal of a son and sub-hierarchy.
     */
    class MWEHTestRemove : public LeafTest
    {
    public:
        MWEHTestRemove() :
            LeafTest("remove")
        {
        }

    protected:
        void test() override
        {
            amt::MultiWayExplicitHierarchy<int> hierarchy;
            this->assert_null(hierarchy.accessRoot());

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
            //        0
            //   /         \
            //   1        x2
            // / | \       |
            // 3 4 5       6
            //   x         ~

            hierarchy.removeSon(one, 1);
            hierarchy.removeSon(root, 1);

            this->assert_equals(static_cast<size_t>(2), hierarchy.degree(one));
            this->assert_equals(5, hierarchy.accessSon(one, 1)->data_);
            this->assert_equals(static_cast<size_t>(1), hierarchy.degree(root));
            this->assert_null(hierarchy.accessSon(root, 1));
        }
    };

    /**
     *  @brief Tests copy constructor, assign and equals.
     */
    class MWEHTestCopyAssignEquals : public LeafTest
    {
    public:
        MWEHTestCopyAssignEquals() :
            LeafTest("copy-assign-equals")
        {
        }

    protected:
        void test() override
        {
            amt::MultiWayExplicitHierarchy<int> hierarchy1;

            auto& root = hierarchy1.emplaceRoot();
            auto& one = hierarchy1.emplaceSon(root, 0);
            auto& two = hierarchy1.emplaceSon(root, 1);
            hierarchy1.emplaceSon(one, 0);
            hierarchy1.emplaceSon(one, 1);
            hierarchy1.emplaceSon(one, 2);
            hierarchy1.emplaceSon(two, 0);

            amt::MultiWayExplicitHierarchy<int> hierarchy2(hierarchy1);
            this->assert_true(hierarchy1.equals(hierarchy2), "Copy constructed hierarchy is the same.");
            hierarchy1.removeSon(root, 1);
            this->assert_false(hierarchy1.equals(hierarchy2), "Modified copy is different.");

            amt::MultiWayExplicitHierarchy<int> hierarchy3;
            hierarchy3.assign(hierarchy1);
            this->assert_true(hierarchy1.equals(hierarchy3), "Assigned hierarchy is the same.");
            hierarchy1.removeSon(one, 0);
            hierarchy1.removeSon(one, 0);
            this->assert_false(hierarchy1.equals(hierarchy3), "Modified assigned hierarchy is different.");
        }
    };

    /**
     *  @brief Tests clear.
     */
    class MWEHTestClear : public LeafTest
    {
    public:
        MWEHTestClear() :
            LeafTest("clear")
        {
        }

    protected:
        void test() override
        {
            amt::MultiWayExplicitHierarchy<int> hierarchy;

            auto& root = hierarchy.emplaceRoot();
            auto& one = hierarchy.emplaceSon(root, 0);
            auto& two = hierarchy.emplaceSon(root, 1);
            hierarchy.emplaceSon(one, 0);
            hierarchy.emplaceSon(one, 1);
            hierarchy.emplaceSon(one, 2);
            hierarchy.emplaceSon(two, 0);

            hierarchy.clear();

            this->assert_equals(static_cast<size_t>(0), hierarchy.size());
            this->assert_null(hierarchy.accessRoot());
            this->assert_true(hierarchy.isEmpty(), "Cleared hierarchy is empty.");
        }
    };

    /**
     * @brief All MultiwayExplicitHierarchy tests.
     */
    class MultiwayExplicitHierarchyTest : public CompositeTest
    {
    public:
        MultiwayExplicitHierarchyTest() :
            CompositeTest("MultiwayExplicitHierarchy")
        {
            this->add_test(std::make_unique<MWEHTestInsert>());
            this->add_test(std::make_unique<MWEHTestAccess>());
            this->add_test(std::make_unique<MWEHTestLevelsCountsDegs>());
            this->add_test(std::make_unique<MWEHTestRemove>());
            this->add_test(std::make_unique<MWEHTestCopyAssignEquals>());
            this->add_test(std::make_unique<MWEHTestClear>());
        }
    };

    /**
     * @brief Tests insertion of root and sons.
     */
    class KWEHTestInsert : public LeafTest
    {
    public:
        KWEHTestInsert() :
            LeafTest("insert")
        {
        }

    protected:
        void test() override
        {
            amt::KWayExplicitHierarchy<int, 3> hierarchy;
            auto& root = hierarchy.emplaceRoot();
            auto& one = hierarchy.emplaceSon(root, 0);
            hierarchy.emplaceSon(root, 1);
            auto& three = hierarchy.emplaceSon(root, 2);
            hierarchy.emplaceSon(one, 0);
            hierarchy.emplaceSon(one, 2);
            hierarchy.emplaceSon(three, 0);
            hierarchy.emplaceSon(three, 1);
            //             0
            //   /         |         \
            //   1         2         3
            // / | \     / | \     / | \
            // 4 _ 5     _ _ _     6 7 _

            this->assert_equals(static_cast<size_t>(8), hierarchy.size());
        }
    };

    /**
     * @brief Tests access of root, son, and parent.
     */
    class KWEHTestAccess : public LeafTest
    {
    public:
        KWEHTestAccess() :
            LeafTest("access")
        {
        }

    protected:
        void test() override
        {
            amt::KWayExplicitHierarchy<int, 3> hierarchy;
            this->assert_null(hierarchy.accessRoot());

            auto& root1 = hierarchy.emplaceRoot();
            root1.data_ = 0;
            auto& one1 = hierarchy.emplaceSon(root1, 0);
            one1.data_ = 1;
            hierarchy.emplaceSon(root1, 1).data_ = 2;
            auto& three1 = hierarchy.emplaceSon(root1, 2);
            three1.data_ = 3;
            hierarchy.emplaceSon(one1, 0).data_ = 4;
            hierarchy.emplaceSon(one1, 2).data_ = 5;
            hierarchy.emplaceSon(three1, 0).data_ = 6;
            hierarchy.emplaceSon(three1, 1).data_ = 7;
            //             0
            //   /         |         \
            //   1         2         3
            // / | \     / | \     / | \
            // 4 _ 5     _ _ _     6 7 _

            auto* root = hierarchy.accessRoot();
            this->assert_not_null(root);
            this->assert_equals(0, root->data_);

            auto* one = hierarchy.accessSon(*root, 0);
            this->assert_not_null(one);
            this->assert_equals(1, one->data_);

            auto* two = hierarchy.accessSon(*root, 1);
            this->assert_not_null(two);
            this->assert_equals(2, two->data_);

            auto* three = hierarchy.accessSon(*root, 2);
            this->assert_not_null(two);
            this->assert_equals(3, three->data_);

            auto* seven = hierarchy.accessSon(*three, 1);
            this->assert_not_null(seven);
            this->assert_equals(7, seven->data_);

            auto* notfour = hierarchy.accessSon(*one, 1);
            this->assert_null(notfour);

            auto* noteight = hierarchy.accessSon(*two, 1);
            this->assert_null(noteight);

            this->assert_equals(three, hierarchy.accessParent(*seven));
            this->assert_equals(root, hierarchy.accessParent(*one));
            this->assert_null(hierarchy.accessParent(*root));
        }
    };

    /**
     * @brief Tests method for calculation of node count, level and degree.
     */
    class KWEHTestLevelCountDeg : public LeafTest
    {
    public:
        KWEHTestLevelCountDeg() :
            LeafTest("level-count-degree")
        {
        }

    protected:
        void test() override
        {
            amt::KWayExplicitHierarchy<int, 3> hierarchy;
            auto& root = hierarchy.emplaceRoot();
            auto& one = hierarchy.emplaceSon(root, 0);
            auto& two = hierarchy.emplaceSon(root, 1);
            auto& three = hierarchy.emplaceSon(root, 2);
            auto& four = hierarchy.emplaceSon(one, 0);
            hierarchy.emplaceSon(one, 2);
            hierarchy.emplaceSon(three, 0);
            auto& seven = hierarchy.emplaceSon(three, 1);
            //             0
            //   /         |         \
            //   1         2         3
            // / | \     / | \     / | \
            // 4 _ 5     _ _ _     6 7 _

            this->assert_equals(static_cast<size_t>(0), hierarchy.level(root));
            this->assert_equals(static_cast<size_t>(1), hierarchy.level(one));
            this->assert_equals(static_cast<size_t>(2), hierarchy.level(seven));

            this->assert_equals(static_cast<size_t>(3), hierarchy.degree(root));
            this->assert_equals(static_cast<size_t>(0), hierarchy.degree(two));
            this->assert_equals(static_cast<size_t>(2), hierarchy.degree(three));
            this->assert_equals(static_cast<size_t>(2), hierarchy.degree(one));

            this->assert_equals(static_cast<size_t>(8), hierarchy.nodeCount(root));
            this->assert_equals(static_cast<size_t>(1), hierarchy.nodeCount(two));
            this->assert_equals(static_cast<size_t>(3), hierarchy.nodeCount(one));
            this->assert_equals(static_cast<size_t>(3), hierarchy.nodeCount(three));
        }
    };

    /**
     *  @brief Tests removal of a son and sub-hierarchy.
     */
    class KWEHTestRemove : public LeafTest
    {
    public:
        KWEHTestRemove() :
            LeafTest("remove")
        {
        }

    protected:
        void test() override
        {
            amt::KWayExplicitHierarchy<int, 3> hierarchy;
            auto& root = hierarchy.emplaceRoot();
            auto& one = hierarchy.emplaceSon(root, 0);
            hierarchy.emplaceSon(root, 1);
            auto& three = hierarchy.emplaceSon(root, 2);
            hierarchy.emplaceSon(one, 0);
            hierarchy.emplaceSon(one, 2);
            hierarchy.emplaceSon(three, 0);
            hierarchy.emplaceSon(three, 1);
            //             0
            //   /         |         \
            //  x1         2         3
            // / | \     / | \     / | \
            // 4 _ 5     _ _ _     6 7 _
            // ~   ~                 x

            hierarchy.removeSon(root, 0);
            hierarchy.removeSon(three, 1);

            this->assert_equals(static_cast<size_t>(4), hierarchy.size());
            this->assert_equals(static_cast<size_t>(2), hierarchy.degree(root));
            this->assert_equals(static_cast<size_t>(1), hierarchy.degree(three));
            this->assert_null(hierarchy.accessSon(root, 0));
            this->assert_null(hierarchy.accessSon(three, 1));
            this->assert_not_null(hierarchy.accessSon(three, 0));
        }
    };

    /**
     *  @brief Tests copy constructor, assign and equals.
     */
    class KWEHTestCopyAssignEquals : public LeafTest
    {
    public:
        KWEHTestCopyAssignEquals() :
            LeafTest("copy-assign-equals")
        {
        }

    protected:
        void test() override
        {
            amt::KWayExplicitHierarchy<int, 3> hierarchy1;
            auto& root = hierarchy1.emplaceRoot();
            auto& one = hierarchy1.emplaceSon(root, 0);
            hierarchy1.emplaceSon(root, 1);
            auto& three = hierarchy1.emplaceSon(root, 2);
            hierarchy1.emplaceSon(one, 0);
            hierarchy1.emplaceSon(one, 2);
            hierarchy1.emplaceSon(three, 0);
            hierarchy1.emplaceSon(three, 1);

            amt::KWayExplicitHierarchy<int, 3> hierarchy2(hierarchy1);
            this->assert_true(hierarchy1.equals(hierarchy2), "Copy constructed hierarchy is the same.");
            hierarchy1.removeSon(root, 1);
            this->assert_false(hierarchy1.equals(hierarchy2), "Modified copy is different.");

            amt::KWayExplicitHierarchy<int, 3> hierarchy3;
            hierarchy3.assign(hierarchy1);
            this->assert_true(hierarchy1.equals(hierarchy3), "Assigned hierarchy is the same.");
            hierarchy1.removeSon(one, 0);
            hierarchy1.removeSon(one, 0);
            this->assert_false(hierarchy1.equals(hierarchy3), "Modified assigned hierarchy is different.");
        }
    };

    /**
     *  @brief Tests clear.
     */
    class KWEHTestClear : public LeafTest
    {
    public:
        KWEHTestClear() :
            LeafTest("clear")
        {
        }

    protected:
        void test() override
        {
            amt::KWayExplicitHierarchy<int, 3> hierarchy;
            auto& root = hierarchy.emplaceRoot();
            auto& one = hierarchy.emplaceSon(root, 0);
            hierarchy.emplaceSon(root, 1);
            auto& three = hierarchy.emplaceSon(root, 2);
            hierarchy.emplaceSon(one, 0);
            hierarchy.emplaceSon(one, 2);
            hierarchy.emplaceSon(three, 0);
            hierarchy.emplaceSon(three, 1);
            hierarchy.clear();

            this->assert_equals(static_cast<size_t>(0), hierarchy.size());
            this->assert_null(hierarchy.accessRoot());
            this->assert_true(hierarchy.isEmpty(), "Cleared hierarchy is empty.");
        }
    };

    /**
     * @brief All KWayExplicitHierarchy tests.
     */
    class KWayExplicitHierarchyTest : public CompositeTest
    {
    public:
        KWayExplicitHierarchyTest() :
            CompositeTest("KWayExplicitHierarchy")
        {
            this->add_test(std::make_unique<KWEHTestInsert>());
            this->add_test(std::make_unique<KWEHTestAccess>());
            this->add_test(std::make_unique<KWEHTestLevelCountDeg>());
            this->add_test(std::make_unique<KWEHTestRemove>());
            this->add_test(std::make_unique<KWEHTestCopyAssignEquals>());
            this->add_test(std::make_unique<KWEHTestClear>());
        }
    };

    /**
     * @brief All ExplicitHierarchy tests.
     */
    class ExplicitHierarchyTest : public CompositeTest
    {
    public:
        ExplicitHierarchyTest() :
            CompositeTest("ExplicitHierarchy")
        {
            this->add_test(std::make_unique<MultiwayExplicitHierarchyTest>());
            this->add_test(std::make_unique<KWayExplicitHierarchyTest>());
        }
    };
}
