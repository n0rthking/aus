#pragma once

#include <tests/_details/test.hpp>
#include <tests/mm/memory_manager.test.h>
#include <tests/mm/compact_memory_manager.test.h>
#include <memory>

namespace ds::tests
{
    class MMTest : public CompositeTest
    {
    public:
        MMTest() :
            CompositeTest("mm")
        {
            this->add_test(std::make_unique<MemoryManagerTest>());
            this->add_test(std::make_unique<CompactMemoryManagerTest>());
        }
    };
}