#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/stack.h>

namespace ds::tests
{
    /**
     * @brief Tests the push operation.
     * \tparam StackT Type of the stack.
     */
    template<class StackT>
    class StackTestPush : public LeafTest
    {
    public:
        StackTestPush() :
            LeafTest("push")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            StackT stack;
            for (int i = 0; i < n; ++i)
            {
                stack.push(i);
                this->assert_equals(i, stack.peek());
            }

            this->assert_equals(static_cast<size_t>(n), stack.size());
        }
    };

    /**
     * @brief Tests the pop operation.
     * \tparam StackT Type of the stack.
     */
    template<class StackT>
    class StackTestPop : public LeafTest
    {
    public:
        StackTestPop() :
            LeafTest("pop")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            StackT stack;
            for (int i = 0; i < n; ++i)
            {
                stack.push(i);
            }

            for (int i = n - 1; i >= 0; --i)
            {
                this->assert_equals(i, stack.pop());
            }

            this->assert_true(stack.isEmpty(), "Stack is empty.");
        }
    };

    /**
     * @brief Tests the clear operation.
     * \tparam StackT Type of the stack.
     */
    template<class StackT>
    class StackTestClear : public LeafTest
    {
    public:
        StackTestClear() :
            LeafTest("clear")
        {
        }

    protected:
        void test() override
        {
            constexpr int n = 10;

            StackT stack;
            for (int i = 0; i < n; ++i)
            {
                stack.push(i);
            }

            stack.clear();

            this->assert_true(stack.isEmpty(), "Cleared stack is empty.");
            this->assert_equals(static_cast<size_t>(0), stack.size());
        }
    };

    /**
     * @brief Tests copy construction, assign, and equals.
     * \tparam StackT Type of the stack.
     */
    template<class StackT>
    class StackTestCopyAssignEquals : public LeafTest
    {
    public:
        StackTestCopyAssignEquals() :
            LeafTest("copy-assign-equals")
        {
        }

    protected:
        void test() override
        {
            StackT stack1;
            constexpr int n = 10;
            for (int i = 0; i < n; ++i)
            {
                stack1.push(i);
            }

            StackT stack2(stack1);
            this->assert_true(stack1.equals(stack2), "Copy constructed stack is the same.");
            stack1.pop();
            this->assert_false(stack1.equals(stack2), "Modified copy is different.");

            StackT stack3;
            stack3.assign(stack1);
            this->assert_true(stack1.equals(stack3), "Assigned stack is the same.");
            stack1.pop();
            stack1.pop();
            this->assert_false(stack1.equals(stack3), "Modified assigned stack is different.");
        }
    };

    /**
     * @brief All stack leaf tests.
     * \tparam StackT Type of the stack.
     */
    template<class StackT>
    class GeneralStackTest : public CompositeTest
    {
    public:
        GeneralStackTest(const std::string& name) :
            CompositeTest(name)
        {
            this->add_test(std::make_unique<StackTestPush<StackT>>());
            this->add_test(std::make_unique<StackTestPop<StackT>>());
            this->add_test(std::make_unique<StackTestClear<StackT>>());
            this->add_test(std::make_unique<StackTestCopyAssignEquals<StackT>>());
        }
    };

    /**
     * @brief All stack tests.
     */
    class StackTest : public CompositeTest
    {
    public:
        StackTest() :
            CompositeTest("Stack")
        {
            this->add_test(std::make_unique<GeneralStackTest<adt::ImplicitStack<int>>>("ImplicitStack"));
            this->add_test(std::make_unique<GeneralStackTest<adt::ExplicitStack<int>>>("ExplicitStack"));
        }
    };
}