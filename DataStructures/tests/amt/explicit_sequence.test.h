#pragma once

#include <tests/_details/test.hpp>
#include <tests/amt/sequence.test.h>
#include <libds/amt/explicit_sequence.h>
#include <memory>

namespace ds::tests
{
    /**
     * @brief All tests for singly linked sequence.
     */
    class SinglyLinkedSequenceTest : public CompositeTest
    {
    public:
        SinglyLinkedSequenceTest() :
            CompositeTest("SinglyLinkedSequence")
        {
            this->add_test(std::make_unique<GenericSequenceTest<amt::SinglyLinkedSequence<int>>>());
        }
    };

    /**
     * @brief All tests for doubly linked sequence.
     */
    class DoubleLinkedSequenceTest : public CompositeTest
    {
    public:
        DoubleLinkedSequenceTest() :
            CompositeTest("DoublyLinkedSequence")
        {
            this->add_test(std::make_unique<GenericSequenceTest<amt::DoublyLinkedSequence<int>>>());
        }
    };

    /**
     * @brief Tests for all explicit sequences.
     */
    class ExplicitSequenceTest : public CompositeTest
    {
    public:
        ExplicitSequenceTest() :
            CompositeTest("ExplicitSequence")
        {
            this->add_test(std::make_unique<SinglyLinkedSequenceTest>());
            this->add_test(std::make_unique<DoubleLinkedSequenceTest>());
        }
    };
}
