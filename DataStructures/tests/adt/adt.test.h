#pragma once

#include <tests/_details/test.hpp>
#include <tests/adt/array.test.h>
#include <tests/adt/list.test.h>
#include <tests/adt/priority_queue.test.h>
#include <tests/adt/queue.test.h>
#include <tests/adt/stack.test.h>
#include <tests/adt/table.test.h>
#include <tests/adt/tree.test.h>
#include <tests/adt/sorts.test.h>

namespace ds::tests
{
    class ADTTest : public CompositeTest
    {
    public:
        ADTTest() :
            CompositeTest("adt")
        {
            this->add_test(std::make_unique<ArraysTest>());
            this->add_test(std::make_unique<ListTest>());
            this->add_test(std::make_unique<PriorityQueueTest>());
            this->add_test(std::make_unique<QueueTest>());
            this->add_test(std::make_unique<StackTest>());
            this->add_test(std::make_unique<TableTest>());
            this->add_test(std::make_unique<TreeTest>());
            this->add_test(std::make_unique<SortTest>());
        }
    };
}