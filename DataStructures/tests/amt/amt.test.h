#pragma once

#include <tests/_details/test.hpp>
#include <tests/amt/implicit_sequence.test.h>
#include <tests/amt/explicit_sequence.test.h>
#include <tests/amt/implicit_hierarchy.test.h>
#include <tests/amt/explicit_hierarchy.test.h>
#include <tests/amt/hierarchy.test.h>
#include <memory>

namespace ds::tests
{
    class AMTTest : public CompositeTest
    {
    public:
        AMTTest() :
            CompositeTest("amt")
        {
            this->add_test(std::make_unique<ImplicitSequenceTest>());
            this->add_test(std::make_unique<ExplicitSequenceTest>());
            this->add_test(std::make_unique<ImplicitHierarchyTest>());
            this->add_test(std::make_unique<ExplicitHierarchyTest>());
            this->add_test(std::make_unique<HierarchyTest>());
        }
    };
}
