#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/tree.h>

namespace ds::tests
{
    class TreeTest : public CompositeTest
    {
    public:
        TreeTest() :
            CompositeTest("Tree")
        {
        }
    };
}