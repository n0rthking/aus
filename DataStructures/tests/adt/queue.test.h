#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/queue.h>

namespace ds::tests
{
    class QueueTest : public CompositeTest
    {
    public:
        QueueTest() :
            CompositeTest("Queue")
        {
        }
    };
}