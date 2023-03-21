#pragma once

#include <libds/amt/implicit_hierarchy.h>
#include <tests/_details/test.hpp>
#include <memory>

namespace ds::tests
{
    /**
     * @brief Tests unsupported methods that should just throw an exception.
     */
    class ImplicitHierarchyTestThrowing : public LeafTest
    {
    public:
        ImplicitHierarchyTestThrowing() :
            LeafTest("unsupported-methods")
        {
        }

    protected:
        auto test() -> void override
        {
            amt::ImplicitHierarchy<int, 3> hierarchy;
            this->assert_throws([&hierarchy]()
                {
                    hierarchy.emplaceRoot();
                });

            this->assert_throws([&hierarchy]()
                {
                    hierarchy.changeRoot(nullptr);
                });

            this->assert_throws([&hierarchy]()
                {
                    hierarchy.emplaceSon(*hierarchy.accessRoot(), 0);
                });

            this->assert_throws([&hierarchy]()
                {
                    hierarchy.changeSon(*hierarchy.accessRoot(), 0, nullptr);
                });

            this->assert_throws([&hierarchy]()
                {
                    hierarchy.removeSon(*hierarchy.accessRoot(), 0);
                });
        }
    };

    /**
     * @brief Tests insertion of the last leaf.
     */
    class ImplicitHierarchyTestInsert : public LeafTest
    {
    public:
        ImplicitHierarchyTestInsert() :
            LeafTest("insertLastLeaf")
        {
        }

    protected:
        void test() override
        {
            constexpr auto n = 9;
            amt::ImplicitHierarchy<int, 3> hierarchy;
            for (int i = 0; i < n; ++i)
            {
                hierarchy.insertLastLeaf().data_ = i;
                this->assert_not_null(hierarchy.accessLastLeaf());
                this->assert_equals(i, hierarchy.accessLastLeaf()->data_);
            }
            //              0
            //      /       |       \
            //      1       2       3
            //    / | \   / |
            //    4 5 6   7 8

            this->assert_equals(static_cast<size_t>(n), hierarchy.size());
        }
    };

    /**
     * @brief Tests access of root, son, parent.
     */
    class ImplicitHierarchyTestAccess : public LeafTest
    {
    public:
        ImplicitHierarchyTestAccess() :
            LeafTest("access")
        {
        }

    protected:
        void test() override
        {
            constexpr auto n = 9;
            amt::ImplicitHierarchy<int, 3> hierarchy;
            this->assert_null(hierarchy.accessRoot());
            for (int i = 0; i < n; ++i)
            {
                hierarchy.insertLastLeaf().data_ = i;
            }
            //              0
            //      /       |       \
            //      1       2       3
            //    / | \   / |
            //    4 5 6   7 8

            this->assert_not_null(hierarchy.accessLastLeaf());
            this->assert_equals(n - 1, hierarchy.accessLastLeaf()->data_);

            auto* root = hierarchy.accessRoot();
            this->assert_not_null(root);
            this->assert_equals(0, root->data_);

            auto* two = hierarchy.accessSon(*root, 1);
            this->assert_not_null(two);
            this->assert_equals(2, two->data_);

            auto* eight = hierarchy.accessSon(*two, 1);
            this->assert_not_null(eight);

            auto* notnine = hierarchy.accessSon(*two, 2);
            this->assert_null(notnine);

            auto* twoagain = hierarchy.accessParent(*eight);
            this->assert_not_null(twoagain);
            this->assert_equals(two, twoagain);
            this->assert_equals(root, hierarchy.accessParent(*twoagain));
        }
    };

    /**
     * @brief Tests method for calculation of node count, level and degree.
     */
    class ImplicitHierarchyTestLevelsCountsDegs : public LeafTest
    {
    public:
        ImplicitHierarchyTestLevelsCountsDegs() :
            LeafTest("level-count-degree")
        {
        }

    protected:
        void test() override
        {
            constexpr auto n = 9;
            amt::ImplicitHierarchy<int, 3> hierarchy;
            for (int i = 0; i < n; ++i)
            {
                hierarchy.insertLastLeaf().data_ = i;
            }
            //              0
            //      /       |       \
            //      1       2       3
            //    / | \   / |
            //    4 5 6   7 8

            this->assert_equals(static_cast<size_t>(n), hierarchy.nodeCount());
            this->assert_equals(static_cast<size_t>(n), hierarchy.nodeCount(*hierarchy.accessRoot()));
            this->assert_equals(static_cast<size_t>(3), hierarchy.nodeCount(*hierarchy.accessSon(*hierarchy.accessRoot(), 1)));

            auto* root = hierarchy.accessRoot();
            auto* two = hierarchy.accessSon(*root, 1);
            auto* eight = hierarchy.accessSon(*two, 1);

            this->assert_equals(static_cast<size_t>(1), hierarchy.level(*two));
            this->assert_equals(static_cast<size_t>(2), hierarchy.level(*eight));

            this->assert_equals(static_cast<size_t>(3), hierarchy.degree(*root));
            this->assert_equals(static_cast<size_t>(2), hierarchy.degree(*two));
            this->assert_equals(static_cast<size_t>(0), hierarchy.degree(*eight));
        }
    };

    /**
     *  @brief Tests removal of the last leaf.
     */
    class ImplicitHierarchyTestRemove : public LeafTest
    {
    public:
        ImplicitHierarchyTestRemove() :
            LeafTest("removeLastLeaf")
        {
        }

    protected:
        void test() override
        {
            constexpr auto n = 9;
            amt::ImplicitHierarchy<int, 3> hierarchy;
            for (int i = 0; i < n; ++i)
            {
                hierarchy.insertLastLeaf().data_ = i;
            }
            //              0
            //      /       |       \
            //      1       2       3
            //    / | \   / |
            //    4 5 6   7 8
            //      x x   x x

            for (int i = 0; i < 4; ++i)
            {
                hierarchy.removeLastLeaf();
                this->assert_equals(n - i - 2, hierarchy.accessLastLeaf()->data_);
            }
        }
    };

    /**
     *  @brief Tests copy constructor and assign.
     */
    class ImplicitHierarchyTestCopyAssign : public LeafTest
    {
    public:
        ImplicitHierarchyTestCopyAssign() :
            LeafTest("copy/assign")
        {
        }

    protected:
        void test() override
        {
            constexpr auto n = 9;
            amt::ImplicitHierarchy<int, 3> hierarchy1;
            for (int i = 0; i < n; ++i)
            {
                hierarchy1.insertLastLeaf().data_ = i;
            }

            amt::ImplicitHierarchy<int, 3> hierarchy2(hierarchy1);
            this->assert_true(hierarchy1.equals(hierarchy2), "Copy constructed hierarchy is the same.");
            hierarchy1.removeLastLeaf();
            this->assert_false(hierarchy1.equals(hierarchy2), "Modified copy is different.");

            amt::ImplicitHierarchy<int, 3> hierarchy3;
            hierarchy3.assign(hierarchy1);
            this->assert_true(hierarchy1.equals(hierarchy3), "Assigned hierarchy is the same.");
            hierarchy1.removeLastLeaf();
            hierarchy1.removeLastLeaf();
            this->assert_false(hierarchy1.equals(hierarchy3), "Modified assigned hierarchy is different.");
        }
    };

    /**
     * @brief All ImplicitHierarchy tests.
     */
    class ImplicitHierarchyTest : public CompositeTest
    {
    public:
        ImplicitHierarchyTest() :
            CompositeTest("ImplicitHierarchy")
        {
            this->add_test(std::make_unique<ImplicitHierarchyTestThrowing>());
            this->add_test(std::make_unique<ImplicitHierarchyTestInsert>());
            this->add_test(std::make_unique<ImplicitHierarchyTestAccess>());
            this->add_test(std::make_unique<ImplicitHierarchyTestLevelsCountsDegs>());
            this->add_test(std::make_unique<ImplicitHierarchyTestRemove>());
            this->add_test(std::make_unique<ImplicitHierarchyTestCopyAssign>());
        }
    };
}