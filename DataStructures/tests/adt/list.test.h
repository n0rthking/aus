#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/list.h>

namespace ds::tests
{
    class ListTest : public CompositeTest
    {
    public:
        ListTest() :
            CompositeTest("List")
        {
        }
    };
}