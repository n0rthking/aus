#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/array.h>

namespace ds::tests
{
    class ArraysTest : public CompositeTest
    {
    public:
        ArraysTest() :
            CompositeTest("Arrays")
        {
        }
    };
}