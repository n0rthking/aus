#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/stack.h>

namespace ds::tests
{
    class StackTest : public CompositeTest
    {
    public:
        StackTest() :
            CompositeTest("Stack")
        {
        }
    };
}