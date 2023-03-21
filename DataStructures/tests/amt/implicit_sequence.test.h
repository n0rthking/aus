#pragma once

#include <tests/_details/test.hpp>
#include <tests/amt/sequence.test.h>
#include <libds/amt/implicit_sequence.h>
#include <memory>

namespace ds::tests
{
    /**
     *  @brief Tests index of next and index of previous.
     */
    class ImplicitSequenceTestIndexOfRelative : public LeafTest
    {
    public:
        ImplicitSequenceTestIndexOfRelative() :
            LeafTest("indexOfNext/Prev")
        {
        }

        void test() override
        {
            constexpr int n = 10;

            amt::ImplicitSequence<int> seq;

            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }

            this->assert_equals(static_cast<size_t>(1), seq.indexOfNext(0));
            this->assert_equals(static_cast<size_t>(4), seq.indexOfPrevious(5));

            this->assert_equals(INVALID_INDEX, seq.indexOfPrevious(0));
            this->assert_equals(INVALID_INDEX, seq.indexOfNext(n - 1));
        }
    };

    /**
     *  @brief Tests index of next and index of previous.
     */
    class CyclicImplicitSequenceTestIndexOfRelative : public LeafTest
    {
    public:
        CyclicImplicitSequenceTestIndexOfRelative() :
            LeafTest("cyclic indexOfNext/Prev")
        {
        }

        void test() override
        {
            constexpr int n = 10;

            amt::CyclicImplicitSequence<int> seq;

            for (int i = 0; i < n; ++i)
            {
                seq.insertLast().data_ = i;
            }

            this->assert_equals(static_cast<size_t>(1), seq.indexOfNext(0));
            this->assert_equals(static_cast<size_t>(4), seq.indexOfPrevious(5));

            this->assert_equals(static_cast<size_t>(n - 1), seq.indexOfPrevious(0));
            this->assert_equals(static_cast<size_t>(0), seq.indexOfNext(n - 1));
        }
    };

    /**
     *  @brief All ImplicitSequenceTests.
     */
    class ImplicitSequenceTest : public CompositeTest
    {
    public:
        ImplicitSequenceTest() :
            CompositeTest("ImplicitSequence")
        {
            this->add_test(std::make_unique<GenericSequenceTest<amt::ImplicitSequence<int>>>());
            this->add_test(std::make_unique<ImplicitSequenceTestIndexOfRelative>());
            this->add_test(std::make_unique<CyclicImplicitSequenceTestIndexOfRelative>());
        }
    };
}
