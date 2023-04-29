#pragma once

#include <algorithm>
#include <libds/adt/table.h>
#include <memory>
#include <random>
#include <tests/_details/test.hpp>

namespace ds::tests
{
    namespace details
    {
        template<class TableT>
        class TableTestBase : public LeafTest
        {
        public:
            TableTestBase(const std::string& name, size_t seed) :
                LeafTest(name),
                rngKey_(seed)
            {
            }

        protected:
            std::vector<int> generateKeys(int count)
            {
                auto keys = std::vector<int>();
                for (auto i = 0; i < count; ++i)
                {
                    keys.push_back(i);
                }
                std::shuffle(begin(keys), end(keys), rngKey_);
                return keys;
            }

            static bool hasKeys(TableT& table, const std::vector<int>& keys)
            {
                for (auto const key : keys)
                {
                    if (!table.contains(key))
                    {
                        return false;
                    }
                }
                return true;
            }

        protected:
            std::mt19937_64 rngKey_;
        };
    }

    /**
     * @brief Tests the insert operation
     * \tparam TableT Table type
     */
    template<class TableT>
    class TableTestInsert : public details::TableTestBase<TableT>
    {
    public:
        TableTestInsert() :
            details::TableTestBase<TableT>("insert", 123)
        {
        }

    protected:
        void test() override
        {
            auto constexpr n = 100;
            auto table = TableT();
            auto const keys = this->generateKeys(n);
            for (auto const key : keys)
            {
                try
                {
                    table.insert(key, key);
                }
                catch (...)
                {
                    this->fail("Insert should not throw on an unique key!");
                }
            }
            this->assert_equals(static_cast<size_t>(keys.size()), table.size());

            for (auto const key : keys)
            {
                try
                {
                    table.insert(key, key);
                    this->fail("Table should throw on a duplicate key");
                }
                catch (...)
                {
                }
            }
        }
    };

    /**
     * @brief Tests the find, tryFind, and contains operations
     * \tparam TableT Table type
     */
    template<class TableT>
    class TableTestLookup : public details::TableTestBase<TableT>
    {
    public:
        TableTestLookup() :
            details::TableTestBase<TableT>("find-tryFind-contains", 456)
        {
        }

    protected:
        void test() override
        {
            auto constexpr n = 100;
            auto table = TableT();
            auto const keys = this->generateKeys(n);
            for (auto const key : keys)
            {
                table.insert(key, key);
            }

            for (auto const key : keys)
            {
                try
                {
                    auto const& data = table.find(key);
                    this->assert_equals(key, data);
                }
                catch (...)
                {
                    this->fail("Table should not throw on an existing key lookup!");
                }

                {
                    auto* data = static_cast<int*>(nullptr);
                    if (!table.tryFind(key, data))
                    {
                        this->fail("Table should find an existing key!");
                    }
                    else
                    {
                        this->assert_equals(key, *data);
                    }
                }

                if (!table.contains(key))
                {
                    this->fail("Table should contain an existing key!");
                }
            }

            for (auto i = 0; i < 10; ++i)
            {
                auto const key = n + i;
                try
                {
                    (void)table.find(key);
                    this->fail("Table should throw on an non-existing key lookup!");
                }
                catch (...)
                {
                }

                {
                    auto* data = static_cast<int*>(nullptr);
                    if (table.tryFind(key, data))
                    {
                        this->fail("Table should not find a non-existing key!");
                    }
                }

                if (table.contains(key))
                {
                    this->fail("Table should not contain a non-existing key!");
                }
            }
        }
    };

    /**
     * @brief Tests the remove operation
     * \tparam TableT Table type
     */
    template<class TableT>
    class TableTestRemove : public details::TableTestBase<TableT>
    {
    public:
        TableTestRemove() :
            details::TableTestBase<TableT>("remove", 789)
        {
        }

    protected:
        void test() override
        {
            auto constexpr n = 100;
            auto table = TableT();
            auto const keys = this->generateKeys(n);
            for (auto const key : keys)
            {
                table.insert(key, key);
            }

            for (auto i = 0; i < 10; ++i)
            {
                auto const key = n + i;
                try
                {
                    (void)table.remove(key);
                    this->fail("Table should not remove non-existing key!");
                }
                catch (...)
                {
                }
            }

            for (auto const key : keys)
            {
                auto const data = table.remove(key);
                this->assert_equals(key, data);
            }

            this->assert_true(table.isEmpty(), "Table is empty");
        }
    };

    /**
     * @brief Tests the clear operation
     * \tparam TableT Table type
     */
    template<class TableT>
    class TableTestClear : public details::TableTestBase<TableT>
    {
    public:
        TableTestClear() :
            details::TableTestBase<TableT>("clear", 147)
        {
        }

    protected:
        void test() override
        {
            auto constexpr n = 100;
            auto table = TableT();
            auto const keys = this->generateKeys(n);
            for (auto const key : keys)
            {
                table.insert(key, key);
            }

            table.clear();
            this->assert_true(table.isEmpty(), "Cleared table is empty");
            this->assert_equals(static_cast<size_t>(0), table.size());
        }
    };

    /**
     * @brief Tests the copy constructor and assign operation
     * \tparam TableT Table type
     */
    template<class TableT>
    class TableTestCopyAssign : public details::TableTestBase<TableT>
    {
    public:
        TableTestCopyAssign() :
            details::TableTestBase<TableT>("copy-assign", 258)
        {
        }

    protected:
        void test() override
        {
            using base = details::TableTestBase<TableT>;

            auto constexpr n = 100;
            auto table1 = TableT();
            auto keys = this->generateKeys(n);
            for (auto const key : keys)
            {
                table1.insert(key, key);
            }

            auto table2 = TableT(table1);
            this->assert_true(base::hasKeys(table2, keys), "Copy constructed table is the same");

            table2.remove(keys.back());
            this->assert_true(base::hasKeys(table1, keys), "Original does not share items with the copy");
            this->assert_false(base::hasKeys(table2, keys), "Modified copy is different");

            table1.clear();
            keys.pop_back();
            this->assert_true(base::hasKeys(table2, keys), "Original does not share anything with the copy");

            table2.clear();
            for (auto const key : keys)
            {
                table2.insert(key, key);
            }
            this->assert_true(base::hasKeys(table2, keys), "Copy still works");

            auto table3 = TableT();
            for (auto i = 0; i < static_cast<int>(keys.size()) / 5; ++i)
            {
                table3.insert(keys[i], keys[i]);
            }
            table3.assign(table2);

            this->assert_true(base::hasKeys(table3, keys), "Copy assigned table is the same");
            table3.remove(keys.back());
            this->assert_true(base::hasKeys(table2, keys), "Original does not share anything with the copy assigned");
            this->assert_false(base::hasKeys(table3, keys), "Modified copy assign is different");
        }
    };

    /**
     * @brief Tests all table operations mixed together
     * \tparam TableT Table type
     */
    template<class TableT>
    class TableTestScenario : public details::TableTestBase<TableT>
    {
    public:
        TableTestScenario() :
            details::TableTestBase<TableT>("scenario", 997)
        {
        }

    protected:
        void test() override
        {
            using base = details::TableTestBase<TableT>;

            auto rngOp = std::mt19937_64(753);
            auto distOp = std::uniform_int_distribution<int>(0, 2);
            auto distKey = std::uniform_int_distribution<int>(0, 9999);
            auto rngKeyIndex = std::mt19937_64(654);
            auto table = TableT();
            auto keys = this->generateKeys(100);
            for (auto const key : keys)
            {
                table.insert(key, key);
            }

            auto const safeOpCount = static_cast<int>(keys.size());
            for (auto i = 0; i < safeOpCount; ++i)
            {
                auto const op = distOp(rngOp);
                switch (op)
                {
                case 0: // insert
                    try
                    {
                        auto const key = distKey(base::rngKey_);
                        table.insert(key, key);
                        keys.push_back(key);
                    }
                    catch (...)
                    {
                    }
                    this->assert_equals(keys.size(), table.size());
                    break;

                case 1: // find
                {
                    auto distKeyIndex = std::uniform_int_distribution<size_t>(0, size(keys) - 1);
                    auto& key = keys[distKeyIndex(rngKeyIndex)];
                    auto data = table.find(key);
                    this->assert_equals(key, data);
                }
                    break;

                case 2: // remove
                {
                    auto distKeyIndex = std::uniform_int_distribution<size_t>(0, size(keys) - 1);
                    auto& key = keys[distKeyIndex(rngKeyIndex)];
                    auto& lastKey = keys.back();
                    auto data = table.remove(key);
                    this->assert_equals(key, data);
                    std::swap(key, lastKey);
                    keys.pop_back();
                    this->assert_equals(keys.size(), table.size());
                }
                    break;

                default:
                    // Unreachable
                    std::terminate();
                }
            }
        }
    };

    /**
     * @brief All table leaf tests
     * \tparam TableT table ty[e
     */
    template<class TableT>
    class GeneralTableTest : public CompositeTest
    {
    public:
        GeneralTableTest(const std::string& name) :
            CompositeTest(name)
        {
            this->add_test(std::make_unique<TableTestInsert<TableT>>());
            this->add_test(std::make_unique<TableTestLookup<TableT>>());
            this->add_test(std::make_unique<TableTestRemove<TableT>>());
            this->add_test(std::make_unique<TableTestClear<TableT>>());
            this->add_test(std::make_unique<TableTestCopyAssign<TableT>>());
            this->add_test(std::make_unique<TableTestScenario<TableT>>());
        }
    };

    /**
     * @brief All sequence table implementations tests
     */
    class SequenceTableTest : public CompositeTest
    {
    public:
        SequenceTableTest() :
            CompositeTest("SequenceTable")
        {
            this->add_test(std::make_unique<GeneralTableTest<adt::UnsortedImplicitSequenceTable<int, int>>>("UnsortedImplicitSequenceTable"));
            this->add_test(std::make_unique<GeneralTableTest<adt::UnsortedExplicitSequenceTable<int, int>>>("UnsortedExplicitSequenceTable"));
            this->add_test(std::make_unique<GeneralTableTest<adt::SortedSequenceTable<int, int>>>("SortedSequenceTable"));
        }
    };

    /**
     * @brief All non-sequence table implementations tests
     */
    class NonSequenceTableTest : public CompositeTest
    {
    public:
        NonSequenceTableTest() :
            CompositeTest("NonSequenceTable")
        {
            this->add_test(std::make_unique<GeneralTableTest<adt::HashTable<int, int>>>("HashTable"));
            this->add_test(std::make_unique<GeneralTableTest<adt::BinarySearchTree<int, int>>>("BinarySearchTree"));
            this->add_test(std::make_unique<GeneralTableTest<adt::Treap<int, int>>>("Treap"));
        }
    };

    /**
     * @brief All table tests
     */
    class TableTest : public CompositeTest
    {
    public:
        TableTest() :
            CompositeTest("Table")
        {
            this->add_test(std::make_unique<GeneralTableTest<adt::UnsortedImplicitSequenceTable<int, int>>>("UnsortedImplicitSequenceTable"));
            this->add_test(std::make_unique<GeneralTableTest<adt::UnsortedExplicitSequenceTable<int, int>>>("UnsortedExplicitSequenceTable"));
            this->add_test(std::make_unique<GeneralTableTest<adt::SortedSequenceTable<int, int>>>("SortedSequenceTable"));
            this->add_test(std::make_unique<GeneralTableTest<adt::HashTable<int, int>>>("HashTable"));
            this->add_test(std::make_unique<GeneralTableTest<adt::BinarySearchTree<int, int>>>("BinarySearchTree"));
            this->add_test(std::make_unique<GeneralTableTest<adt::Treap<int, int>>>("Treap"));
        }
    };
}