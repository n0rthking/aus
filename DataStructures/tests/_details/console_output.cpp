#include <tests/_details/console_output.hpp>
#include <tests/_details/test.hpp>
#include <algorithm>
#include <iostream>
#include <string_view>

namespace ds::tests
{
    namespace
    {
        auto test_result_to_color (TestResult const result) -> Color
        {
            switch (result)
            {
            case TestResult::Pass:
                return Color::Green;

            case TestResult::Fail:
                return Color::Red;

            case TestResult::Partial:
                return Color::Yellow;

            default:
                return Color::Default;
            }
        }
    }

    TestOutputterVisitor::TestOutputterVisitor
        (ConsoleOutputType o) :
        otype_ (o)
    {
    }

    auto TestOutputterVisitor::visit
        (LeafTest& t) -> void
    {
        if (prefix_.empty())
        {
            console_.println(t.name(), test_result_to_color(t.result()));
        }

        prefix_ += "    ";
        if (otype_ != ConsoleOutputType::NoLeaf)
        {
            for (auto const& r : t.output())
            {
                console_.print(prefix_);
                switch (r.type_)
                {
                case TestMessageType::Pass:
                    console_.print("pass", Color::Green);
                    break;

                case TestMessageType::Fail:
                    console_.print("fail", Color::Red);
                    break;

                case TestMessageType::Info:
                    console_.print("info", Color::Blue);
                    break;

                default:
                    break;
                }
                console_.print(" ");
                console_.println(r.text_);
            }
        }

        if (prefix_.size() >= 4)
        {
            prefix_.resize(std::max(static_cast<std::size_t>(0ul), prefix_.size() - 4));
        }

        if (prefix_.size() >= 4)
        {
            prefix_.resize(std::max(static_cast<std::size_t>(0ul), prefix_.size() - 4));
        }
    }

    auto TestOutputterVisitor::visit
        (CompositeTest& t) -> void
    {
        if (prefix_.empty())
        {
            console_.println(t.name(), test_result_to_color(t.result()));
        }

        for (auto const& st : t.subtests())
        {
            console_.print(prefix_);
            console_.print("+>  ");
            console_.println(st->name(), test_result_to_color(st->result()));

            prefix_ += &st == &t.subtests().back() ? "    " :  "|   ";
            st->accept(*this);
        }

        if (prefix_.size() >= 4)
        {
            prefix_.resize(std::max(static_cast<std::size_t>(0ul), prefix_.size() - 4));
        }
    }
}