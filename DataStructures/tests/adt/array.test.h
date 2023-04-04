#pragma once

#include <tests/_details/test.hpp>
#include <libds/adt/array.h>

namespace ds::tests
{
    /**
     * @brief Tests simple queries.
     */
    class ArrayTestGetters : public LeafTest
    {
    public:
        ArrayTestGetters() :
            LeafTest("getters")
        {
        }

    protected:
        void test() override
        {
            const long long base = -3;
            const size_t size = 10;
            const auto arr = adt::Array<int>(adt::Dimension(base, size));
            this->assert_equals(base, arr.getBase());
            this->assert_equals(size, arr.size());
            this->assert_false(arr.isEmpty(), "Array is not empty.");
        }
    };

    /**
     * @brief Tests set and access.
     */
    class ArrayTestSetAccess : public LeafTest
    {
    public:
        ArrayTestSetAccess() :
            LeafTest("set-access")
        {
        }

    protected:
        void test() override
        {
            const int base = -3;
            const size_t size = 10;
            auto arr = adt::Array<int>(adt::Dimension(base, size));
            for (int i = base; i < base + static_cast<int>(size); ++i)
            {
                arr.set(i, i);
            }

            for (int i = base; i < base + static_cast<int>(size); ++i)
            {
                this->assert_equals(arr.access(i), i);
            }

            this->assert_throws([=, &arr]()
                {
                    arr.set(-1, base + static_cast<int>(size) + 10);
                });

            this->assert_throws([=, &arr]()
                {
                    (void)arr.access(base + static_cast<int>(size) + 10);
                });
        }
    };

    /**
     * @brief Tests copy constructor, assign, and equals.
     */
    class ArrayTestCopyAssignEquals : public LeafTest
    {
    public:
        ArrayTestCopyAssignEquals() :
            LeafTest("copy-assign-equals")
        {
        }

    protected:
        void test() override
        {
            const int base = -3;
            const size_t size = 10;
            auto arr1 = adt::Array<int>(adt::Dimension(base, size));
            for (int i = base; i < base + static_cast<int>(size); ++i)
            {
                arr1.set(i, i);
            }

            const auto arr2(arr1);
            this->assert_true(arr1.equals(arr2), "Copy constructed array is the same.");
            arr1.set(-1, base);
            this->assert_false(arr1.equals(arr2), "Modified copy is different.");

            auto arr3 = adt::Array<int>(adt::Dimension(base, size));
            arr3.assign(arr1);
            this->assert_true(arr1.equals(arr3), "Assigned array is the same.");
            arr3.set(-1, base + 1);
            arr3.set(-1, base + 2);
            this->assert_false(arr1.equals(arr3), "Modified assigned array is different.");

            auto arr4 = adt::Array<int>(adt::Dimension(base, size + 10));
            this->assert_throws([&arr4, &arr3]()
                {
                    arr4.assign(arr3);
                });
        }
    };

    /**
     * @brief Tests simple queries.
     */
    class MatrixTestGetters : public LeafTest
    {
    public:
        MatrixTestGetters() :
            LeafTest("getters")
        {
        }

    protected:
        void test() override
        {
            const auto d1 = adt::Dimension(-4, 5);
            const auto d2 = adt::Dimension(3, 10);
            const auto matrix = adt::CompactMatrix<int>(d1, d2);
            this->assert_equals(d1, matrix.getDimension1());
            this->assert_equals(d2, matrix.getDimension2());
            this->assert_equals(d1.getSize() * d2.getSize(), matrix.size());
            this->assert_false(matrix.isEmpty(), "CompactMatrix is not empty.");
        }
    };

    /**
     * @brief Tests set and access.
     */
    class MatrixTestSetAccess : public LeafTest
    {
    public:
        MatrixTestSetAccess() :
            LeafTest("set-access")
        {
        }

    protected:
        void test() override
        {
            const auto d1 = adt::Dimension(-4, 5);
            const auto d2 = adt::Dimension(3, 10);
            auto matrix = adt::CompactMatrix<int>(d1, d2);
            for (long long i = d1.getBase(); i < d1.getBase() + static_cast<long long>(d1.getSize()); ++i)
            {
                for (long long j = d2.getBase(); j < d2.getBase() + static_cast<long long>(d2.getSize()); ++j)
                {
                    matrix.set(static_cast<int>(i * j), i, j);
                }
            }

            for (long long i = d1.getBase(); i < d1.getBase() + static_cast<long long>(d1.getSize()); ++i)
            {
                for (long long j = d2.getBase(); j < d2.getBase() + static_cast<long long>(d2.getSize()); ++j)
                {
                    this->assert_equals(static_cast<int>(i * j), matrix.access(i, j));
                }
            }

            this->assert_throws([&]()
                {
                    matrix.set(-1, d1.getBase() - 10, d2.getBase() + 1);
                });

            this->assert_throws([&]()
                {
                    matrix.set(-1, d1.getBase() + 1, d2.getBase() - 10);
                });

            this->assert_throws([&]()
                {
                    matrix.set(-1, d1.getBase() - 10, d2.getBase() - 10);
                });

            this->assert_throws([&]()
                {
                    (void)matrix.access(d1.getBase() - 10, d2.getBase() + 1);
                });

            this->assert_throws([&]()
                {
                    (void)matrix.access(d1.getBase() + 1, d2.getBase() - 10);
                });

            this->assert_throws([&]()
                {
                    (void)matrix.access(d1.getBase() - 10, d2.getBase() - 10);
                });
        }
    };

    /**
     * @brief Tests copy constructor, assign, and equals.
     */
    class MatrixTestCopyAssignEquals : public LeafTest
    {
    public:
        MatrixTestCopyAssignEquals() :
            LeafTest("copy-assign-equals")
        {
        }

    protected:
        void test() override
        {
            const auto d1 = adt::Dimension(-4, 5);
            const auto d2 = adt::Dimension(3, 10);
            auto matrix1 = adt::CompactMatrix<int>(d1, d2);
            for (long long i = d1.getBase(); i < d1.getBase() + static_cast<long long>(d1.getSize()); ++i)
            {
                for (long long j = d2.getBase(); j < d2.getBase() + static_cast<long long>(d2.getSize()); ++j)
                {
                    matrix1.set(static_cast<int>(i * j), i, j);
                }
            }

            const auto matrix2(matrix1);
            this->assert_true(matrix1.equals(matrix2), "Copy constructed matrix is the same.");
            matrix1.set(-1, d1.getBase() + 1, d2.getBase() + 1);
            this->assert_false(matrix1.equals(matrix2), "Modified copy is different.");

            auto matrix3 = adt::CompactMatrix<int>(d1, d2);
            matrix3.assign(matrix1);
            this->assert_true(matrix1.equals(matrix3), "Assigned matrix is the same.");
            matrix3.set(-1, d1.getBase() + 1, d2.getBase() + 1);
            matrix3.set(-1, d1.getBase() + 2, d2.getBase() + 2);
            this->assert_false(matrix1.equals(matrix3), "Modified assigned matrix is different.");

            this->assert_throws([&]()
                {
                    auto matrix4 = adt::CompactMatrix<int>(adt::Dimension(d1.getBase() + 1, d1.getSize() - 1), d2);
                    matrix4.assign(matrix3);
                });

            this->assert_throws([&]()
                {
                    auto matrix4 = adt::CompactMatrix<int>(d1, adt::Dimension(d2.getBase() + 1, d2.getSize() - 1));
                    matrix4.assign(matrix3);
                });
        }
    };

    /**
     * @brief All array tests.
     */
    class ArraysTest : public CompositeTest
    {
    public:
        ArraysTest() :
            CompositeTest("Arrays")
        {
            this->add_test(std::make_unique<ArrayTestGetters>());
            this->add_test(std::make_unique<ArrayTestSetAccess>());
            this->add_test(std::make_unique<ArrayTestCopyAssignEquals>());
            this->add_test(std::make_unique<MatrixTestGetters>());
            this->add_test(std::make_unique<MatrixTestSetAccess>());
            this->add_test(std::make_unique<MatrixTestCopyAssignEquals>());
        }
    };
}