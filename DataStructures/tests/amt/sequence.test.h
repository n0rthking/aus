#pragma once

#include <tests/_details/test.hpp>
#include <libds/amt/sequence.h>
#include <memory>

namespace ds::tests
{
    /**
     * @brief Tests calculation of elements position.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestCalculateIndex : public LeafTest
    {
    public:
        SequenceTestCalculateIndex() :
            LeafTest("calculateIndex")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }

            auto* three = seq.access(2);
            const size_t threeIndex = seq.calculateIndex(*three);
            this->assert_equals(static_cast<size_t>(2), threeIndex);

            decltype(three) x;
            this->assert_equals(INVALID_INDEX, seq.calculateIndex(*x));
        }
    };

    /**
     * @brief Tests insertion of the first and the last element.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestInsertEnds : public LeafTest
    {
    public:
        SequenceTestInsertEnds() :
            LeafTest("insertEnds")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
                seq.insertFirst().data_ = i;
                this->assert_equals(i, seq.accessFirst()->data_);
                this->assert_equals(i, seq.accessLast()->data_);
            }
        }
    };

    /**
     * @brief Tests insertion at random positions.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestInsertRandom : public LeafTest
    {
    public:
        SequenceTestInsertRandom() :
            LeafTest("insertRandom")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n / 2; ++i)
            {
                seq.insert(i).data_ = 2 * i;
            }
            // 0 2 4 6 8

            for (int i = 0; i < n / 2; ++i)
            {
                const int k = 2 * i + 1;
                seq.insert(k).data_ = k;
            }
            //  v v v v v
            // 0 2 4 6 8

            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(i, seq.access(i)->data_);
            }
            // 0 1 2 3 4 5 6 7 8 9
        }
    };

    /**
     * @brief Tests insertion before a block.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestInsertBefore : public LeafTest
    {
    public:
        SequenceTestInsertBefore() :
            LeafTest("insertBefore")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n / 2; ++i)
            {
                seq.insert(i).data_ = 2 * i;
            }
            // 0 2 4 6 8

            for (int i = 0; i < n / 2 - 1; ++i)
            {
                const int k = 2 * i + 1;
                auto* block = seq.access(k);
                seq.insertBefore(*block).data_ = k;
            }
            //   v- v- v- v-
            // 0  2  4  6  8

            for (int i = 0; i < n - 1; ++i)
            {
                this->assert_equals(i, seq.access(i)->data_);
            }
            // 0 1 2 3 4 5 6 7 8
        }
    };

    /**
     * @brief Tests insertion after a block.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestInsertAfter : public LeafTest
    {
    public:
        SequenceTestInsertAfter() :
            LeafTest("insertAfter")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n / 2; ++i)
            {
                seq.insert(i).data_ = 2 * i;
            }
            // 0 2 4 6 8

            for (int i = 0; i < n / 2; ++i)
            {
                const int k = 2 * i + 1;
                auto* block = seq.access(k - 1);
                seq.insertAfter(*block).data_ = k;
            }
            // -v -v -v -v -v
            // 0  2  4  6  8

            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(i, seq.access(i)->data_);
            }
            // 0 1 2 3 4 5 6 7 8
        }
    };

    /**
     * @brief Tests access of the first and the last element.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestAccessEnds : public LeafTest
    {
    public:
        SequenceTestAccessEnds() :
            LeafTest("accessEnds")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;

            this->assert_null(seq.accessFirst());
            this->assert_null(seq.accessLast());

            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }

            this->assert_equals(0, seq.accessFirst()->data_, "accessFirst");
            this->assert_equals(n - 1, seq.accessLast()->data_, "accessLast");
        }
    };

    /**
     * @brief Tests access of elements at an arbitrary position.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestAccessRandom : public LeafTest
    {
    public:
        SequenceTestAccessRandom() :
            LeafTest("accessRandom")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }

            this->assert_null(seq.access(n));
            this->assert_null(seq.access(static_cast<size_t>(n) + 10));
            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(i, seq.access(static_cast<size_t>(i))->data_);
            }
        }
    };

    /**
     * @brief Test access of next and previous to block element.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestAccessPrevNext : public LeafTest
    {
    public:
        SequenceTestAccessPrevNext() :
            LeafTest("accessPrevNext")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }

            this->assert_null(seq.accessPrevious(*seq.accessFirst()));
            this->assert_null(seq.accessNext(*seq.accessLast()));
            this->assert_equals(1, seq.accessNext(*seq.accessFirst())->data_);
            this->assert_equals(n - 2, seq.accessPrevious(*seq.accessLast())->data_);
            this->assert_equals(4, seq.accessNext(*seq.access(3))->data_);
            this->assert_equals(7, seq.accessPrevious(*seq.access(8))->data_);
        }
    };

    /**
     * @brief Tests removal of the first and the last element.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestRemoveEnds : public LeafTest
    {
    public:
        SequenceTestRemoveEnds() :
            LeafTest("removeEnds")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            seq.insertFirst().data_ = 0;
            for (int i = 1; i < n; ++i)
            {
                seq.insertLast().data_ = i;
                seq.insertFirst().data_ = i;
            }
            // 9 8 .. 2 1 0 1 2 .. 8 9

            for (int i = 0; i < n - 1; ++i)
            {
                seq.removeFirst();
                seq.removeLast();
                this->assert_equals(n - i - 2, seq.accessFirst()->data_);
                this->assert_equals(n - i - 2, seq.accessLast()->data_);
            }
            // 8 7 .. 2 1 0 1 2 .. 7 8
            // 7 6 .. 2 1 0 1 2 .. 6 7
            //           ...
            //            0
        }
    };

    /**
     * @brief Tests removal of element at an arbitrary position.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestRemoveRandom : public LeafTest
    {
    public:
        SequenceTestRemoveRandom() :
            LeafTest("removeRandom")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }
            // 0 1 2 3 4 5 6 7 8 9

            for (int i = 1; i <= n / 2; ++i)
            {
                seq.remove(i);
            }
            // 0 2 4 6 8

            for (int i = 0; i < n / 2; ++i)
            {
                this->assert_equals(2 * i, seq.access(i)->data_);
            }
        }
    };

    /**
     * @brief Tests removal of next to block element.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestRemoveNext : public LeafTest
    {
    public:
        SequenceTestRemoveNext() :
            LeafTest("removeNext")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }
            // 0 1 2 3 4 5 6 7 8 9

            for (int i = 0; i < n / 2; ++i)
            {
                auto* block = seq.access(i);
                seq.removeNext(*block);
            }
            // --v --v --v --v --v
            // 0 1 2 3 4 5 6 7 8 9
            //   x   x   x   x   x

            for (int i = 0; i < n / 2; ++i)
            {
                this->assert_equals(2 * i, seq.access(i)->data_);
            }
            // 0 2 4 6 8
        }
    };

    /**
     * @brief Tests removal of previous to block element.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestRemovePrevious : public LeafTest
    {
    public:
        SequenceTestRemovePrevious() :
            LeafTest("removePrevious")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }
            // 0 1 2 3 4 5 6 7 8 9

            for (int i = 1; i <= n / 2; ++i)
            {
                auto* block = seq.access(i);
                seq.removePrevious(*block);
            }
            // v-- v-- v-- v-- v--
            // 0 1 2 3 4 5 6 7 8 9
            // x   x   x   x   x

            for (int i = 0; i < n / 2; ++i)
            {
                this->assert_equals(2 * i + 1, seq.access(i)->data_);
            }
            // 1 3 5 7 9
        }
    };

    /**
     * @brief Tests processing of all elements in forward order.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestProcessForward : public LeafTest
    {
    public:
        SequenceTestProcessForward() :
            LeafTest("processForward")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }
            // 0 1 2 3 4 5 6 7 8 9

            int processed = 0;
            seq.processBlocksForward(seq.accessFirst(), [this, &processed](const amt::MemoryBlock<int>* block)
                {
                    this->assert_equals(processed, block->data_);
                    ++processed;
                });
            this->assert_equals(n, processed);
        }
    };

    /**
     * @brief Tests processing of all elements in backward order.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestProcessBackward : public LeafTest
    {
    public:
        SequenceTestProcessBackward() :
            LeafTest("processBackward")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }
            // 0 1 2 3 4 5 6 7 8 9

            int processed = 0;
            seq.processBlocksBackward(seq.accessLast(), [this, &processed, n](const amt::MemoryBlock<int>* block)
                {
                    this->assert_equals(n - processed - 1, block->data_);
                    ++processed;
                });
            this->assert_equals(n, processed);
        }
    };

    /**
     * @brief Tests finding a block with property.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestFindWithProperty : public LeafTest
    {
    public:
        SequenceTestFindWithProperty() :
            LeafTest("findBlockWithProperty")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }
            // 0 1 2 3 4 5 6 7 8 9

            constexpr int intToFind = 5;
            amt::MemoryBlock<int>* found = seq.findBlockWithProperty([=](const amt::MemoryBlock<int>* block)
                {
                    return block->data_ == intToFind;
                });
            this->assert_not_null(found);
            this->assert_equals(intToFind, found->data_);

            amt::MemoryBlock<int>* notFound = seq.findBlockWithProperty([](amt::MemoryBlock<int>*)
                {
                    return false;
                });
            this->assert_null(notFound);
        }
    };

    /**
     * @brief Tests finding block previous to a block with property.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestFindPrevToWithProperty : public LeafTest
    {
    public:
        SequenceTestFindPrevToWithProperty() :
            LeafTest("findPrevToBlockWithProperty")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }
            // 0 1 2 3 4 5 6 7 8 9

            constexpr int intToFind = 5;
            amt::MemoryBlock<int>* found = seq.findPreviousToBlockWithProperty([=](const amt::MemoryBlock<int>* block)
                {
                    return block->data_ == intToFind;
                });
            this->assert_not_null(found);
            this->assert_equals(intToFind - 1, found->data_);
            this->assert_null(seq.findPreviousToBlockWithProperty([](amt::MemoryBlock<int>*)
                {
                    return false;
                }));
            this->assert_null(seq.findPreviousToBlockWithProperty([](const amt::MemoryBlock<int>* block)
                {
                    return block->data_ == 0;
                }));
        }
    };

    /**
     * @brief Tests iterator of the sequence.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestIterators : public LeafTest
    {
    public:
        SequenceTestIterators()
            : LeafTest("iterator")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }
            // 0 1 2 3 4 5 6 7 8 9

            int expectedData = 0;
            auto it = seq.begin();
            auto endIt = seq.end();
            while (it != endIt)
            {
                if (*it != expectedData)
                {
                    this->fail("Iterator dereferenced to invalid value!");
                    break;
                }
                ++expectedData;
                ++it;
            }

            if (expectedData != n)
            {
                this->fail("Iterator did not access all elements!");
            }
            else
            {
                this->pass("Iterator visited all elements");
            }
        }
    };

    /**
     * @brief Tests the clear operation.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestClear : public LeafTest
    {
    public:
        SequenceTestClear() :
            LeafTest("clear")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            SequenceT seq;
            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }

            seq.clear();

            this->assert_true(seq.isEmpty(), "Cleared sequence is empty.");
            this->assert_equals(static_cast<size_t>(0), seq.size());
        }
    };

    /**
     * @brief Tests copy construction, assign, and equals.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class SequenceTestCopyAssignEquals : public LeafTest
    {
    public:
        SequenceTestCopyAssignEquals() :
            LeafTest("copy-assign-equals")
        {
        }

    protected:
        void test() override
        {
            SequenceT seq1;
            constexpr int n = 10;
            for (int i = 0; i < n; ++i)
            {
                seq1.insertLast().data_ = i;
            }

            SequenceT seq2(seq1);
            this->assert_true(seq1.equals(seq2), "Copy constructed sequence is the same.");
            seq1.removeLast();
            this->assert_false(seq1.equals(seq2), "Modified copy is different.");

            SequenceT seq3;
            seq3.assign(seq1);
            this->assert_true(seq1.equals(seq3), "Assigned sequence is the same.");
            seq1.removeLast();
            seq1.removeLast();
            this->assert_false(seq1.equals(seq3), "Modified assigned sequence is different.");
        }
    };

    /**
     * @brief All generic sequence leaf tests.
     * \tparam SequenceT Type of the sequence. Either explicit or implicit.
     */
    template<class SequenceT>
    class GenericSequenceTest : public CompositeTest
    {
    public:
        GenericSequenceTest() :
            CompositeTest("GenericSequenceTest")
        {
            this->add_test(std::make_unique<SequenceTestCalculateIndex<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestInsertEnds<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestInsertRandom<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestInsertBefore<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestInsertAfter<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestAccessEnds<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestAccessRandom<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestAccessPrevNext<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestRemoveEnds<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestRemoveRandom<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestRemoveNext<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestRemovePrevious<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestProcessForward<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestProcessBackward<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestFindWithProperty<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestFindPrevToWithProperty<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestIterators<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestClear<SequenceT>>());
            this->add_test(std::make_unique<SequenceTestCopyAssignEquals<SequenceT>>());
        }
    };
}
