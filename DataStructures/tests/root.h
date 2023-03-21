#pragma once

#include <tests/_details/test.hpp>
#include <tests/adt/adt.test.h>
#include <tests/amt/amt.test.h>
#include <tests/mm/mm.test.h>
#include <memory>

namespace ds::tests
{
    class RootTest : public CompositeTest
    {
    public:
        RootTest() :
            CompositeTest("ds")
        {
            this->add_test(std::make_unique<MMTest>());
            this->add_test(std::make_unique<AMTTest>());
            this->add_test(std::make_unique<ADTTest>());
        }
    };
}
