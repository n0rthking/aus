#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/priority_queue.h>
#include <random>
#include <type_traits>

namespace ds::tests
{
    namespace details
    {
        template<class PrioQueueT>
        class PrioQueueTestBase : public LeafTest
        {
        public:
            PrioQueueTestBase(const std::string& name) :
                LeafTest(name),
                rngPrio_(144),
                distPrio_(0, 10)
            {
            }

        protected:
            int generateRandomPriority()
            {
                return distPrio_(rngPrio_);
            }

            PrioQueueT makeQueue(const int n)
            {
                if constexpr (std::is_same_v<PrioQueueT, adt::TwoLists<int, int>>)
                {
                    return PrioQueueT(n);
                }
                else
                {
                    return PrioQueueT();
                }
            }

            bool bruteforceEquals(PrioQueueT lhs, PrioQueueT rhs)
            {
                if (lhs.size() != rhs.size())
                {
                    return false;
                }

                while (!lhs.isEmpty())
                {
                    const int l = lhs.pop();
                    const int r = rhs.pop();
                    if (l != r)
                    {
                        return false;
                    }
                }

                return true;
            }

        private:
            std::mt19937_64 rngPrio_;
            std::uniform_int_distribution<int> distPrio_;
        };
    }

    /**
     * @brief Tests the push operation.
     * \tparam PrioQueueT Type of the priority queue.
     */
    template<class PrioQueueT>
    class PrioQueueTestPush : public details::PrioQueueTestBase<PrioQueueT>
    {
    public:
        PrioQueueTestPush() :
            details::PrioQueueTestBase<PrioQueueT>("push")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 30;

            PrioQueueT queue = details::PrioQueueTestBase<PrioQueueT>::makeQueue(n);
            int maxPeek = this->generateRandomPriority();
            queue.push(maxPeek, maxPeek);
            for (int i = 0; i < n - 1; ++i)
            {
                const int newPriority = this->generateRandomPriority();
                queue.push(newPriority, newPriority);
                maxPeek = newPriority < maxPeek ? newPriority : maxPeek;
                const int newPeek = queue.peek();
                this->assert_true(maxPeek <= newPeek, "Peek returns max priority");
            }

            this->assert_equals(static_cast<size_t>(n), queue.size());
        }
    };

    /**
     * @brief Tests the pop operation.
     * \tparam PrioQueueT Type of the priority queue.
     */
    template<class PrioQueueT>
    class PrioQueueTestPop : public details::PrioQueueTestBase<PrioQueueT>
    {
    public:
        PrioQueueTestPop() :
            details::PrioQueueTestBase<PrioQueueT>("pop")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 30;
            PrioQueueT queue = details::PrioQueueTestBase<PrioQueueT>::makeQueue(n);
            for (int i = 0; i < n; ++i)
            {
                const int newPriority = this->generateRandomPriority();
                queue.push(newPriority, newPriority);
            }

            int prevPrio = queue.peek();
            for (int i = 0; i < n; ++i)
            {
                const int popped = queue.pop();
                this->assert_true(popped >= prevPrio, "Popped prio is smaller than previously popped");
                prevPrio = popped;
            }

            this->assert_true(queue.isEmpty(), "Queue is empty");
        }
    };

    /**
     * @brief Tests the clear operation.
     * \tparam PrioQueueT Type of the priority queue.
     */
    template<class PrioQueueT>
    class PrioQueueTestClear : public details::PrioQueueTestBase<PrioQueueT>
    {
    public:
        PrioQueueTestClear() :
            details::PrioQueueTestBase<PrioQueueT>("clear")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 30;
            PrioQueueT queue = details::PrioQueueTestBase<PrioQueueT>::makeQueue(n);
            for (int i = 0; i < n; ++i)
            {
                const int newPriority = this->generateRandomPriority();
                queue.push(newPriority, newPriority);
            }

            queue.clear();

            this->assert_true(queue.isEmpty(), "Cleared queue is empty.");
            this->assert_equals(static_cast<size_t>(0), queue.size());
        }
    };

    /**
     * @brief Tests copy construction, assign.
     * \tparam PrioQueueT Type of the queue.
     */
        template<class PrioQueueT>
        class PrioQueueTestCopyAssign : public details::PrioQueueTestBase<PrioQueueT>
        {
        public:
            PrioQueueTestCopyAssign() :
                details::PrioQueueTestBase<PrioQueueT>("copy-assign")
            {
            }

        protected:
            void test() override
            {
                constexpr int n = 30;
                PrioQueueT queue1 = details::PrioQueueTestBase<PrioQueueT>::makeQueue(n);;
                for (int i = 0; i < n; ++i)
                {
                    const int newPriority = this->generateRandomPriority();
                    queue1.push(newPriority, newPriority);
                }

                PrioQueueT queue2(queue1);
                this->assert_true(this->bruteforceEquals(queue1, queue2), "Copy constructed queue is the same.");
                queue1.pop();
                this->assert_false(this->bruteforceEquals(queue1, queue2), "Modified copy is different.");

                PrioQueueT queue3 = details::PrioQueueTestBase<PrioQueueT>::makeQueue(n);;;
                queue3.assign(queue1);
                this->assert_true(this->bruteforceEquals(queue1, queue3), "Assigned queue is the same.");
                queue1.pop();
                queue1.pop();
                this->assert_false(this->bruteforceEquals(queue1, queue3), "Modified assigned queue is different.");

                PrioQueueT queue4(queue1);
                PrioQueueT queue5(queue1);
                queue4.push(40, 40);
                queue4.push(50, 50);
                queue5.push(50, 50);
                queue5.push(40, 40);
                queue4.pop();
                queue5.pop();
                this->assert_true(this->bruteforceEquals(queue4, queue5), "Modified copies are the same");
            }
        };

    /**
     * @brief All priority queue leaf tests.
     * \tparam PrioQueueT Type of the priority queue.
     */
    template<class PrioQueueT>
    class GeneralPrioQueueTest : public CompositeTest
    {
    public:
        GeneralPrioQueueTest(const std::string& name) :
            CompositeTest(name)
        {
            this->add_test(std::make_unique<PrioQueueTestPush<PrioQueueT>>());
            this->add_test(std::make_unique<PrioQueueTestPop<PrioQueueT>>());
            this->add_test(std::make_unique<PrioQueueTestClear<PrioQueueT>>());
            this->add_test(std::make_unique<PrioQueueTestCopyAssign<PrioQueueT>>());
        }
    };

    /**
     * @brief All priority queue tests.
     */
    class PriorityQueueTest : public CompositeTest
    {
    public:
        PriorityQueueTest() :
            CompositeTest("PriorityQueue")
        {
            this->add_test(std::make_unique<GeneralPrioQueueTest<adt::UnsortedImplicitSequencePriorityQueue<int, int>>>("UnsortedImplicit"));
            this->add_test(std::make_unique<GeneralPrioQueueTest<adt::UnsortedExplicitSequencePriorityQueue<int, int>>>("UnsortedExplicit"));
            this->add_test(std::make_unique<GeneralPrioQueueTest<adt::SortedImplicitSequencePriorityQueue<int, int>>>("SortedImplicit"));
            this->add_test(std::make_unique<GeneralPrioQueueTest<adt::SortedExplicitSequencePriorityQueue<int, int>>>("SortedExplicit"));
            this->add_test(std::make_unique<GeneralPrioQueueTest<adt::TwoLists<int, int>>>("TwoLists"));
            this->add_test(std::make_unique<GeneralPrioQueueTest<adt::BinaryHeap<int, int>>>("BinaryHeap"));
        }
    };
}