#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/sorts.h>

namespace ds::tests
{
    class SortTest : public CompositeTest
    {
    public:
        SortTest() :
            CompositeTest("Sort")
        {
        }
    };
}