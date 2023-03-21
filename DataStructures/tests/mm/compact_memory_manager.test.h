#pragma once

#include <tests/_details/test.hpp>
#include <libds/mm/compact_memory_manager.h>
#include <memory>

namespace ds::tests
{
    /**
     * @brief Tests getter for allocated count.
     */
    class CompactMemoryManagerTestGetters : public LeafTest
    {
    public:
        CompactMemoryManagerTestGetters() :
            LeafTest("getters")
        {
        }

    protected:
        void test() override
        {
            mm::CompactMemoryManager<int> manager;
            const int n = 10;

            for (int i = 0; i < n; ++i)
            {
                *manager.allocateMemory() = i;
            }

            this->assert_equals(static_cast<std::size_t>(n), manager.getAllocatedBlockCount());
        }
    };

    /**
     * @brief Tests allocation at the end.
     */
    class CompactMemoryManagerTestAllocate : public LeafTest
    {
    public:
        CompactMemoryManagerTestAllocate() :
            LeafTest("allocate")
        {
        }

    protected:
        void test() override
        {
            mm::CompactMemoryManager<int> manager;
            const int n = 10;

            for (int i = 0; i < n; ++i)
            {
                *manager.allocateMemory() = i;
            }

            for (int i = 0; i < n; ++i)
            {
                int* expectedPtr = &manager.getBlockAt(0) + i;
                int* actualPtr = &manager.getBlockAt(i);
                this->assert_equals(expectedPtr, actualPtr);
                this->assert_equals(i, *actualPtr);
            }
        }
    };

    /**
     * @brief Tests allocation at an arbitrary index.
     */
    class CompactMemoryManagerTestAllocateAt : public LeafTest
    {
    public:
        CompactMemoryManagerTestAllocateAt() :
            LeafTest("allocateAt")
        {
        }

    protected:
        void test() override
        {
            mm::CompactMemoryManager<int> manager;
            const int n = 10;

            for (int i = 0; i < n / 2; ++i)
            {
                *manager.allocateMemoryAt(i) = 2 * i;
            }

            for (int i = 0; i < n / 2; ++i)
            {
                const int k = 2 * i + 1;
                *manager.allocateMemoryAt(k) = k;
            }

            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(i, manager.getBlockAt(i));
            }
        }
    };

    /**
     * @brief Tests deallocation at the end.
     */
    class CompactMemoryManagerTestRelease : public LeafTest
    {
    public:
        CompactMemoryManagerTestRelease() :
            LeafTest("release")
        {
        }

    protected:
        void test() override
        {
            mm::CompactMemoryManager<int> manager;
            const int n = 10;

            for (int i = 0; i < n; ++i)
            {
                *manager.allocateMemory() = i;
            }

            manager.releaseMemory();

            this->assert_equals(n - 2, manager.getBlockAt(n - 2));
        }
    };

    /**
     * @brief Tests deallocation at an arbitrary index.
     */
    class CompactMemoryManagerTestReleaseAt : public LeafTest
    {
    public:
        CompactMemoryManagerTestReleaseAt() :
            LeafTest("releaseAt")
        {
        }

    protected:
        void test() override
        {
            mm::CompactMemoryManager<int> manager;
            const int n = 10;

            for (int i = 0; i < n; ++i)
            {
                *manager.allocateMemory() = i;
            }

            manager.releaseMemoryAt(n - 1);
            manager.releaseMemoryAt(0);
            manager.releaseMemoryAt(2);

            this->assert_equals(1, manager.getBlockAt(0));
            this->assert_equals(8, manager.getBlockAt(n - 3));
            this->assert_equals(4, manager.getBlockAt(2));
        }
    };

    /**
     * @brief Tests deallocation starting at a pointer.
     */
    class CompactMemoryManagerTestReleasePtr : public LeafTest
    {
    public:
        CompactMemoryManagerTestReleasePtr() :
            LeafTest("releasePtr")
        {
        }

    protected:
        void test() override
        {
            mm::CompactMemoryManager<int> manager;
            const int n = 10;

            for (int i = 0; i < n; ++i)
            {
                *manager.allocateMemory() = i;
            }

            manager.releaseMemory(std::addressof(manager.getBlockAt(4)));

            this->assert_equals(static_cast<std::size_t>(4), manager.getAllocatedBlockCount());
            this->assert_equals(3, manager.getBlockAt(3));
        }
    };

    /**
     * @brief Tests assign.
     */
    class CompactMemoryManagerTestAssign : public LeafTest
    {
    public:
        CompactMemoryManagerTestAssign() :
            LeafTest("assign")
        {
        }

    protected:
        void test() override
        {
            const int n = 10;

            mm::CompactMemoryManager<int> manager1;
            for (int i = 0; i < n; ++i)
            {
                *manager1.allocateMemory() = i;
            }

            mm::CompactMemoryManager<int> manager2;
            manager2.assign(manager1);
            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(manager1.getBlockAt(i), manager2.getBlockAt(i));
            }

            for (int i = 0; i < n; ++i)
            {
                manager1.getBlockAt(i) = 0;
            }

            for (int i = 0; i < n; ++i)
            {
                this->assert_equals(manager2.getBlockAt(i), i);
            }
        }
    };

    class CompactMemoryManagerTestEquals : public LeafTest
    {
    public:
        CompactMemoryManagerTestEquals() :
            LeafTest("equals")
        {
        }

    protected:
        void test() override
        {
            const int n = 10;

            mm::CompactMemoryManager<int> manager1;
            for (int i = 0; i < n; ++i)
            {
                *manager1.allocateMemory() = i;
            }

            mm::CompactMemoryManager<int> manager2;
            for (int i = 0; i < n; ++i)
            {
                *manager2.allocateMemory() = i;
            }

            this->assert_true(manager1.equals(manager2), "equals");
            manager2.getBlockAt(4) = 10;
            this->assert_false(manager1.equals(manager2), "not equals");
        }
    };

    /**
     * @brief Tests calculate address.
     */
    class CompactMemoryManagerTestCalculateAddress : public LeafTest
    {
    public:
        CompactMemoryManagerTestCalculateAddress() :
            LeafTest("calculateAddress")
        {
        }

    protected:
        void test() override
        {
            const int n = 10;

            mm::CompactMemoryManager<int> manager;
            for (int i = 0; i < n; ++i)
            {
                *manager.allocateMemory() = i;
            }

            int& third = manager.getBlockAt(3);
            void* const expectedAddress = std::addressof(third);
            this->assert_equals(expectedAddress, manager.calculateAddress(third));
        }
    };

    /**
     * @brief Tests calculateIndex.
     */
    class CompactMemoryManagerTestCalculateIndex : public LeafTest
    {
    public:
        CompactMemoryManagerTestCalculateIndex() :
            LeafTest("calculateIndex")
        {
        }

    protected:
        auto test() -> void override
        {
            const int n = 10;

            mm::CompactMemoryManager<int> manager;
            for (int i = 0; i < n; ++i)
            {
                *manager.allocateMemory() = i;
            }

            int& third = manager.getBlockAt(3);
            this->assert_equals(static_cast<std::size_t>(3), manager.calculateIndex(third));

            int x = 100;
            this->assert_equals(INVALID_INDEX, manager.calculateIndex(x));
        }
    };

    /**
     * @brief Tests swap.
     */
    class CompactMemoryManagerSwap : public LeafTest
    {
    public:
        CompactMemoryManagerSwap() :
            LeafTest("swap")
        {
        }

    protected:
        void test() override
        {
            const int n = 10;

            mm::CompactMemoryManager<int> manager;
            for (int i = 0; i < n; ++i)
            {
                *manager.allocateMemory() = i;
            }

            for (int i = 0; i < n / 2; ++i)
            {
                manager.swap(i, n - i - 1);
            }

            for (int i = 0; i < n / 2; ++i)
            {
                this->assert_equals(n - i - 1, manager.getBlockAt(i));
            }
        }
    };

    /**
     * @brief All CompactMemoryManager leaf tests.
     */
    class CompactMemoryManagerTest : public CompositeTest
    {
    public:
        CompactMemoryManagerTest() :
            CompositeTest("CompactMemoryManager")
        {
            this->add_test(std::make_unique<CompactMemoryManagerTestGetters>());
            this->add_test(std::make_unique<CompactMemoryManagerTestAllocate>());
            this->add_test(std::make_unique<CompactMemoryManagerTestAllocateAt>());
            this->add_test(std::make_unique<CompactMemoryManagerTestRelease>());
            this->add_test(std::make_unique<CompactMemoryManagerTestReleaseAt>());
            this->add_test(std::make_unique<CompactMemoryManagerTestReleasePtr>());
            this->add_test(std::make_unique<CompactMemoryManagerTestAssign>());
            this->add_test(std::make_unique<CompactMemoryManagerTestEquals>());
            this->add_test(std::make_unique<CompactMemoryManagerTestCalculateAddress>());
            this->add_test(std::make_unique<CompactMemoryManagerTestCalculateIndex>());
            this->add_test(std::make_unique<CompactMemoryManagerSwap>());
        }
    };
}
