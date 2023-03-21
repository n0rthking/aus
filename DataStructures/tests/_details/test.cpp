#include <tests/_details/test.hpp>
#include <tests/_details/console_output.hpp>
#include <algorithm>
#include <exception>

namespace ds::tests
{
// TestMessageType:

    auto test_message_type_to_string(TestMessageType const t) -> std::string
    {
        switch (t)
        {
        case TestMessageType::Fail:
            return "Fail";

        case TestMessageType::Info:
            return "Info";

        case TestMessageType::Pass:
            return "Pass";

        default:
            return "<UnknownMessageType>";
        }
    }

// Test:

    Test::Test
        (std::string name) :
        name_ (std::move(name))
    {
    }

    auto Test::name
        () const -> std::string_view
    {
        return name_;
    }

// LeafTest:

    namespace
    {
        struct test_failed_exception
        {
        };
    }

    LeafTest::LeafTest
        (std::string name, AssertPolicy policy) :
        Test (std::move(name)),
        assertPolicy_ (policy)
    {
    }

    auto LeafTest::run
        () -> void
    {
        try
        {
            results_.clear();
            this->test();
        }
        catch (test_failed_exception)
        {
            this->info("Terminated after failed assertion.");
        }
        catch (const std::exception& e)
        {
            using namespace std::string_literals;
            this->log_fail("Unhandled exception: "s + e.what());
        }
        catch (...)
        {
            this->log_fail("Unhandled exception.");
        }
    }

    auto LeafTest::result
        () const -> TestResult
    {
        auto asserts = std::vector<TestMessage>();
        std::copy_if(begin(results_), end(results_), std::back_inserter(asserts),
            [](TestMessage const& m)
            {
                return m.type_ != TestMessageType::Info;
            });

        auto const is_pass = [](TestMessage const& m)
        {
            return m.type_ == TestMessageType::Pass;
        };

        return
            results_.empty()
                ? TestResult::NotEvaluated :
            std::all_of(begin(asserts), end(asserts), is_pass)
                ? TestResult::Pass :
            std::none_of(begin(asserts), end(asserts), is_pass)
                ? TestResult::Fail :
            TestResult::Partial;
    }

    auto LeafTest::reset() -> void
    {
        results_.clear();
    }


    auto LeafTest::output
        () const -> std::vector<TestMessage> const&
    {
        return results_;
    }

    auto LeafTest::accept
        (IVisitor& v) -> void
    {
        v.visit(*this);
    }

    auto LeafTest::assert_true
        (bool const b, std::string m) -> void
    {
        if (b)
        {
            this->pass(std::move(m));
        }
        else
        {
            this->fail(std::move(m));
        }
    }

    auto LeafTest::assert_false
        (bool const b, std::string m) -> void
    {
        this->assert_true(not b, std::move(m));
    }

    auto LeafTest::assert_null
        (std::nullptr_t) -> void
    {
        this->pass("Null literal is indeed null");
    }

    auto LeafTest::assert_not_null
        (std::nullptr_t) -> void
    {
        this->fail("Null literal cannot be not null");
    }

    auto LeafTest::assert_nullopt
        (std::nullopt_t) -> void
    {
        this->pass("Nullopt literal is indeed nullopt");
    }

    auto LeafTest::assert_has_value
        (std::nullopt_t) -> void
    {
        this->fail("Nullopt literal never has value");
    }

    auto LeafTest::info
        (std::string m) -> void
    {
        results_.emplace_back(
            TestMessage {TestMessageType::Info, std::move(m)}
        );
    }

    auto LeafTest::fail
        (std::string m) -> void
    {
        this->log_fail(std::move(m));
        if (assertPolicy_ == AssertPolicy::StopAtFirstFail)
        {
            throw test_failed_exception();
        }
    }

    auto LeafTest::pass
        (std::string m) -> void
    {
        results_.emplace_back(
            TestMessage {TestMessageType::Pass, std::move(m)}
        );
    }

    auto LeafTest::log_fail
        (std::string m) -> void
    {
        results_.emplace_back(
            TestMessage {TestMessageType::Fail, std::move(m)}
        );
    }

// CompositeTest:

    CompositeTest::CompositeTest
        (std::string name) :
        Test (std::move(name))
    {
    }

    auto CompositeTest::accept
        (IVisitor& v) -> void
    {
        v.visit(*this);
    }

    auto CompositeTest::run
        () -> void
    {
        for (auto& t : tests_)
        {
            t->run();
        }
    }

    auto CompositeTest::result
        () const -> TestResult
    {
        auto results = std::vector<TestResult>();
        std::transform(begin(tests_), end(tests_), std::back_inserter(results),
            [](std::unique_ptr<Test> const& test)
            {
                return test->result();
            });

        auto const didfail = [](TestResult const r)
        {
            return r == TestResult::Fail;
        };
        auto const didpass = [](TestResult const r)
        {
            return r == TestResult::Pass;
        };
        auto const didntev = [](TestResult const r)
        {
            return r == TestResult::NotEvaluated;
        };

        return
            std::all_of(begin(results), end(results), didntev)
                ? TestResult::NotEvaluated :
            std::all_of(begin(results), end(results), didfail)
                ? TestResult::Fail :
            std::all_of(begin(results), end(results), didpass)
                ? TestResult::Pass :
            TestResult::Partial;
    }

    auto CompositeTest::reset() -> void
    {
        for (std::unique_ptr<Test> const& subTest : tests_)
        {
            subTest->reset();
        }
    }

    auto CompositeTest::subtests
        () const -> std::vector<std::unique_ptr<Test>> const&
    {
        return tests_;
    }

    auto CompositeTest::subtests
        () -> std::vector<std::unique_ptr<Test>>&
    {
        return tests_;
    }

    auto CompositeTest::add_test
        (std::unique_ptr<Test> t) -> void
    {
        tests_.emplace_back(std::move(t));
    }

// Free functions:

    auto console_print_results (Test& t, ConsoleOutputType o) -> void
    {
        auto out = TestOutputterVisitor(o);
        t.accept(out);
    }
}