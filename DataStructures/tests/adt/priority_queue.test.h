#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/priority_queue.h>

namespace ds::tests
{
    class PriorityQueueTest : public CompositeTest
    {
    public:
        PriorityQueueTest() :
            CompositeTest("PriorityQueue")
        {
        }
    };
}