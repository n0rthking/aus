#pragma once

#include <algorithm>
#include <functional>
#include <libds/amt/implicit_sequence.h>
#include <libds/adt/sorts.h>
#include <random>
#include <string>
#include <tests/_details/test.hpp>

namespace ds::tests
{
    namespace details
    {
        auto constexpr makeReverseComparator = [](auto const& cmp)
        {
            return [cmp](auto const& l, auto const& r)
            {
                return cmp(r, l);
            };
        };

        class RandomStringKeyGenerator
        {
        public:
            RandomStringKeyGenerator(std::mt19937_64& seeder) :
                rngLength_(seeder()),
                rngChar_(seeder()),
                distLen_(std::size_t{2}, std::size_t{10}),
                distChar_(static_cast<int>('a'), static_cast<int>('z'))
            {
            }

            amt::ImplicitSequence<std::string> operator()(std::size_t const n)
            {
                auto keys = amt::ImplicitSequence<std::string>(n, false);
                for (std::size_t i = 0; i < n; ++i)
                {
                    auto key = std::string();
                    auto const keyLength = distLen_(rngLength_);
                    key.reserve(keyLength);
                    for (std::size_t j = 0; j < keyLength; ++j)
                    {
                        key += static_cast<char>(distChar_(rngChar_));
                    }
                    keys.insertLast().data_ = std::move(key);
                }
                return keys;
            }

        private:
            std::mt19937_64 rngLength_;
            std::mt19937_64 rngChar_;
            std::uniform_int_distribution<std::size_t> distLen_;
            std::uniform_int_distribution<int> distChar_;
        };

        class RandomIntKeyGenerator
        {
        public:
            RandomIntKeyGenerator(std::mt19937_64& seeder) :
                rngKey_(seeder())
            {
            }

            amt::ImplicitSequence<int> operator()(std::size_t const n)
            {
                auto keys = amt::ImplicitSequence<int>(n, false);
                for (auto i = 0; i < n; ++i)
                {
                    keys.insertLast().data_ = distKey_(rngKey_);
                }
                return keys;
            }

        private:
            std::mt19937_64 rngKey_;
            std::uniform_int_distribution<int> distKey_;
        };

        struct StringLengthComparator
        {
            bool operator()(std::string const& l, std::string const& r) const
            {
                return l.size() < r.size();
            }
        };
    }

    /**
     * @brief Tests sorting algorithm
     * @tparam SortT Sort type
     * @tparam Comparator Comparator type
     * @tparam KeyGenerator Function objects that generates sequence of n keys
     */
    template<
        class SortT,
        class Comparator,
        class KeyGenerator
    >
    class GenericSortTest : public LeafTest
    {
    public:
        GenericSortTest(const std::string& name, KeyGenerator keyGenerator, Comparator cmp, int elementCount) :
            tests::LeafTest(name),
            keyGenerator_(std::move(keyGenerator)),
            cmp_(std::move(cmp)),
            elementCount_(elementCount)
        {
        }

    protected:
        void test() override
        {
            auto keys = keyGenerator_(elementCount_);
            auto sorter = SortT();
            sorter.sort(keys, cmp_);
            this->assert_true(std::is_sorted(keys.begin(), keys.end(), cmp_), "Is sorted.");
        }

    private:
        KeyGenerator keyGenerator_;
        Comparator cmp_;
        int elementCount_;
    };

    /**
     * @brief Creates tests with int and string keys for @p SortT
     * @tparam SortT Sort type
     */
    template<template<class> class SortT>
    class MultiCmpSortTest : public CompositeTest
    {
    public:
        MultiCmpSortTest(std::string const& name, std::mt19937_64& seeder, int const elementCount) :
            tests::CompositeTest(name)
        {
            using namespace details;
            this->add_test(std::make_unique<GenericSortTest<SortT<int>, std::less<int>, RandomIntKeyGenerator>>(
                "int-keys-asc",
                RandomIntKeyGenerator(seeder),
                std::less<int>(),
                elementCount
            ));
            this->add_test(std::make_unique<GenericSortTest<SortT<int>, std::greater<int>, RandomIntKeyGenerator>>(
                "int-keys-desc",
                RandomIntKeyGenerator(seeder),
                std::greater<int>(),
                elementCount
            ));
            this->add_test(std::make_unique<GenericSortTest<SortT<std::string>, std::less<std::string>, RandomStringKeyGenerator>>(
                "string-keys-asc",
                RandomStringKeyGenerator(seeder),
                std::less<std::string>(),
                elementCount
            ));
            this->add_test(std::make_unique<GenericSortTest<SortT<std::string>, std::greater<std::string>, RandomStringKeyGenerator>>(
                "string-keys-desc",
                RandomStringKeyGenerator(seeder),
                std::greater<std::string>(),
                elementCount
            ));
            this->add_test(std::make_unique<GenericSortTest<SortT<std::string>, StringLengthComparator, RandomStringKeyGenerator>>(
                "string-keys-length-asc",
                RandomStringKeyGenerator(seeder),
                StringLengthComparator(),
                elementCount
            ));
            this->add_test(std::make_unique<GenericSortTest<SortT<std::string>, decltype(makeReverseComparator(StringLengthComparator())), RandomStringKeyGenerator>>(
                "string-keys-length-desc",
                RandomStringKeyGenerator(seeder),
                makeReverseComparator(StringLengthComparator()),
                elementCount
            ));
        }
    };

    /**
     * @brief Special test case for radix sort
     */
    class RadixSortTest : public GenericSortTest<adt::RadixSort<int, int>, std::less<int>, details::RandomIntKeyGenerator>
    {
    public:
        RadixSortTest(std::mt19937_64& seeder) :
            GenericSortTest(
                "RadixSort",
                details::RandomIntKeyGenerator(seeder),
                std::less<int>(),
                1'000
            )
        {
        }
    };

    /**
     * @brief All sort tests
     */
    class SortTest : public CompositeTest
    {
    public:
        SortTest() :
            CompositeTest("Sort")
        {
            auto seeder = std::mt19937_64(247);
            this->add_test(std::make_unique<MultiCmpSortTest<adt::SelectSort>>("SelectSort", seeder, 100));
            this->add_test(std::make_unique<MultiCmpSortTest<adt::BubbleSort>>("BubbleSort", seeder, 100));
            this->add_test(std::make_unique<MultiCmpSortTest<adt::InsertSort>>("InsertSort", seeder, 100));
            this->add_test(std::make_unique<MultiCmpSortTest<adt::QuickSort>>("QuickSort", seeder, 10'000));
            this->add_test(std::make_unique<MultiCmpSortTest<adt::HeapSort>>("HeapSort", seeder, 10'000));
            this->add_test(std::make_unique<MultiCmpSortTest<adt::ShellSort>>("ShellSort", seeder, 10'000));
            this->add_test(std::make_unique<MultiCmpSortTest<adt::MergeSort>>("MergeSort", seeder, 10'000));
            this->add_test(std::make_unique<RadixSortTest>(seeder));
        }
    };
}