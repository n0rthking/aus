#pragma once

#include <tests/_details/test.hpp>
#include <tests/mm/memory_manager.test.h>
#include <memory>
#include <vector>

namespace ds::tests
{
    class MemoryManagerTestAllocate : public LeafTest
    {
    public:
        MemoryManagerTestAllocate() :
            LeafTest("allocate")
        {
        }

    protected:
        void test() override
        {
            mm::MemoryManager<int> manager;
            int* ip = manager.allocateMemory();

            const int data = 42;
            *ip = data;
            this->assert_equals(data, *ip);
            manager.releaseMemory(ip);
        }
    };

    class MemoryManagerTestRelease : public LeafTest
    {
    public:
        MemoryManagerTestRelease() :
            LeafTest("release")
        {
        }

    protected:
        void test() override
        {
            mm::MemoryManager<int> manager;
            int* ip1 = manager.allocateMemory();
            int* ip2 = manager.allocateMemory();

            const int data = 42;
            *ip1 = data;
            *ip2 = data;

            manager.releaseMemory(ip1);
            manager.releaseAndSetNull(ip2);

            this->assert_null(ip2);
        }
    };

    class MemoryManagerTestCounts : public LeafTest
    {
    public:
        MemoryManagerTestCounts() :
            LeafTest("count")
        {
        }

    protected:
        void test() override
        {
            const std::size_t elemcount = 10;
            std::vector<int*> elems;
            elems.reserve(elemcount);
            mm::MemoryManager<int> manager;

            for (std::size_t i = 0; i < elemcount; ++i)
            {
                elems.push_back(manager.allocateMemory());
            }

            this->assert_equals(elemcount, manager.getAllocatedBlockCount());

            for (auto* elem : elems)
            {
                manager.releaseMemory(elem);
            }
        }
    };

    class MemoryManagerTest : public CompositeTest
    {
    public:
        explicit MemoryManagerTest()
            : CompositeTest("MemoryManager")
        {
            this->add_test(std::make_unique<MemoryManagerTestAllocate>());
            this->add_test(std::make_unique<MemoryManagerTestRelease>());
            this->add_test(std::make_unique<MemoryManagerTestCounts>());
        }
    };
}