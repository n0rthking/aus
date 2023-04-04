#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/list.h>

namespace ds::tests
{
    /**
     * @brief Tests insertion of the first and the last element.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestInsertEnds : public LeafTest
    {
    public:
        ListTestInsertEnds() :
            LeafTest("insertEnds")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            for (int i = 0; i < n; ++i)
            {
                list.insertFirst(i);
                list.insertLast(i);
                this->assert_equals(i, list.accessFirst());
                this->assert_equals(i, list.accessLast());
            }
        }
    };

    /**
     * @brief Tests insertion at random positions.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestInsertRandom : public LeafTest
    {
    public:
        ListTestInsertRandom() :
            LeafTest("insertRandom")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            for (int i = 0; i < n / 2; ++i)
            {
                list.insert(2 * i, i);
            }
            // 0 2 4 6 8

            for (int i = 0; i < n / 2; ++i)
            {
                const int k = 2 * i + 1;
                list.insert(k, k);
            }
            //  v v v v v
            // 0 2 4 6 8

            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(i, list.access(i));
            }
            // 0 1 2 3 4 5 6 7 8 9
        }
    };

    /**
     * @brief Tests calculation of elements position.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestCalculateIndex : public LeafTest
    {
    public:
        ListTestCalculateIndex() :
            LeafTest("calculateIndex")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            for (int i = 0; i < n; ++i)
            {
                list.insertLast(i);
            }

            this->assert_equals(static_cast<size_t>(3), list.calculateIndex(3));
            this->assert_equals(INVALID_INDEX, list.calculateIndex(n + 100));
        }
    };

    /**
     * @brief Tests access of the first and the last element.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestAccessEnds : public LeafTest
    {
    public:
        ListTestAccessEnds() :
            LeafTest("accessEnds")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;

            this->assert_throws([&, this]()
                {
                    list.accessFirst();
                });

            this->assert_throws([&, this]()
                {
                    list.accessLast();
                });

            for (int i = 0; i < n; ++i)
            {
                list.insertLast(i);
            }

            this->assert_equals(0, list.accessFirst(), "accessFirst");
            this->assert_equals(n - 1, list.accessLast(), "accessLast");
        }
    };

    /**
     * @brief Tests access of elements at an arbitrary position.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestAccessRandom : public LeafTest
    {
    public:
        ListTestAccessRandom() :
            LeafTest("accessRandom")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            for (int i = 0; i < n; ++i)
            {
                list.insertLast(i);
            }

            this->assert_throws([&list, this, n]()
                {
                    list.access(static_cast<size_t>(n));
                });

            this->assert_throws([&list, this, n]()
                {
                    list.access(static_cast<size_t>(n) + 100);
                });

            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(i, list.access(static_cast<size_t>(i)));
            }
        }
    };

    /**
     * @brief Tests setting of elements at an arbitrary position.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestSet : public LeafTest
    {
    public:
        ListTestSet() :
            LeafTest("set")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            for (int i = 0; i < n; ++i)
            {
                list.insertLast(i);
            }

            this->assert_throws([&list, this, n]()
                {
                    list.set(n, - 1);
                });

            for (int i = 0; i < n; ++i)
            {
                list.set(i, static_cast<size_t>(2 * i));
            }

            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(2 * i, list.access(i));
            }
        }
    };

    /**
     * @brief Tests removal of the first and the last element.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestRemoveEnds : public LeafTest
    {
    public:
        ListTestRemoveEnds() :
            LeafTest("removeEnds")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            list.insertFirst(0);
            for (int i = 1; i < n; ++i)
            {
                list.insertLast(i);
                list.insertFirst(i);
            }
            // 9 8 .. 2 1 0 1 2 .. 8 9

            for (int i = 0; i < n - 1; ++i)
            {
                list.removeFirst();
                list.removeLast();
                this->assert_equals(n - i - 2, list.accessFirst());
                this->assert_equals(n - i - 2, list.accessLast());
            }
            // 8 7 .. 2 1 0 1 2 .. 7 8
            // 7 6 .. 2 1 0 1 2 .. 6 7
            //           ...
            //            0
        }
    };

    /**
     * @brief Tests removal of element at an arbitrary position.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestRemoveRandom : public LeafTest
    {
    public:
        ListTestRemoveRandom() :
            LeafTest("removeRandom")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            for (int i = 0; i < n; ++i)
            {
                list.insertLast(i);
            }
            // 0 1 2 3 4 5 6 7 8 9

            for (int i = 1; i <= n / 2; ++i)
            {
                list.remove(i);
            }
            // 0 2 4 6 8

            for (int i = 0; i < n / 2; ++i)
            {
                this->assert_equals(2 * i, list.access(i));
            }
        }
    };

    /**
     * @brief Tests iterator of the list.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestIterators : public LeafTest
    {
    public:
        ListTestIterators()
            : LeafTest("iterator")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            for (int i = 0; i < n; ++i)
            {
                list.insertLast(i);
            }
            // 0 1 2 3 4 5 6 7 8 9

            int expectedData = 0;
            auto it = list.begin();
            auto endIt = list.end();
            while (it != endIt)
            {
                this->assert_equals(expectedData, *it);
                ++expectedData;
                ++it;
            }
        }
    };

    /**
     * @brief Tests the clear operation.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestClear : public LeafTest
    {
    public:
        ListTestClear() :
            LeafTest("clear")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            ListT list;
            for (int i = 0; i < n; ++i)
            {
                list.insertLast(i);
            }

            list.clear();

            this->assert_true(list.isEmpty(), "Cleared list is empty.");
            this->assert_equals(static_cast<size_t>(0), list.size());
        }
    };

    /**
     * @brief Tests copy construction, assign, and equals.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class ListTestCopyAssignEquals : public LeafTest
    {
    public:
        ListTestCopyAssignEquals() :
            LeafTest("copy-assign-equals")
        {
        }

    protected:
        void test() override
        {
            ListT list1;
            constexpr int n = 10;
            for (int i = 0; i < n; ++i)
            {
                list1.insertLast(i);
            }

            ListT list2(list1);
            this->assert_true(list1.equals(list2), "Copy constructed list is the same.");
            list1.removeLast();
            this->assert_false(list1.equals(list2), "Modified copy is different.");

            ListT list3;
            list3.assign(list1);
            this->assert_true(list1.equals(list3), "Assigned list is the same.");
            list1.removeLast();
            list1.removeLast();
            this->assert_false(list1.equals(list3), "Modified assigned list is different.");
        }
    };

    /**
     * @brief All list leaf tests.
     * \tparam ListT Type of the list.
     */
    template<class ListT>
    class GeneralListTest : public CompositeTest
    {
    public:
        GeneralListTest(const std::string& name) :
            CompositeTest(name)
        {
            this->add_test(std::make_unique<ListTestInsertEnds<ListT>>());
            this->add_test(std::make_unique<ListTestInsertRandom<ListT>>());
            this->add_test(std::make_unique<ListTestCalculateIndex<ListT>>());
            this->add_test(std::make_unique<ListTestAccessEnds<ListT>>());
            this->add_test(std::make_unique<ListTestAccessRandom<ListT>>());
            this->add_test(std::make_unique<ListTestSet<ListT>>());
            this->add_test(std::make_unique<ListTestRemoveEnds<ListT>>());
            this->add_test(std::make_unique<ListTestRemoveRandom<ListT>>());
            this->add_test(std::make_unique<ListTestIterators<ListT>>());
            this->add_test(std::make_unique<ListTestClear<ListT>>());
            this->add_test(std::make_unique<ListTestCopyAssignEquals<ListT>>());
        }
    };

    /**
     * @brief All list tests.
     */
    class ListTest : public CompositeTest
    {
    public:
        ListTest() :
            CompositeTest("List")
        {
            this->add_test(std::make_unique<GeneralListTest<adt::ImplicitList<int>>>("ImplicitList"));
            this->add_test(std::make_unique<GeneralListTest<adt::SinglyLinkedList<int>>>("SinglyLinkedList"));
            this->add_test(std::make_unique<GeneralListTest<adt::DoublyLinkedList<int>>>("DoublyLinkedList"));
        }
    };
}