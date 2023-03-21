#pragma once

#include <tests/_details/console.hpp>
#include <tests/_details/visitors.hpp>
#include <string>

namespace ds::tests
{
    /**
     *  @brief Specifies level of details in the console output.
     */
    enum class ConsoleOutputType
    {
        Full,
        NoLeaf
    };

    class LeafTest;
    class CompositeTest;

    /**
     *  @brief Prints results of all tests in the hierarchy.
     */
    class TestOutputterVisitor : public IVisitor
    {
    public:
        TestOutputterVisitor(ConsoleOutputType);
        auto visit (LeafTest&) -> void override;
        auto visit (CompositeTest&) -> void override;

    private:
        Console console_;
        std::string prefix_;
        ConsoleOutputType otype_;
    };
}