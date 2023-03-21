#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/table.h>
#include <memory>

namespace ds::tests
{
    class SequenceTableTest : public CompositeTest
    {
    public:
        SequenceTableTest() :
            CompositeTest("SequenceTable")
        {
        }
    };

    class NonSequenceTableTest : public CompositeTest
    {
    public:
        NonSequenceTableTest() :
            CompositeTest("NonSequenceTable")
        {
        }
    };

    class TableTest : public CompositeTest
    {
    public:
        TableTest() :
            CompositeTest("Table")
        {
            this->add_test(std::make_unique<SequenceTableTest>());
            this->add_test(std::make_unique<NonSequenceTableTest>());
        }
    };
}