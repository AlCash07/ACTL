/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/cp/diagnostics/to_string.hpp>
#include <actl/macros.hpp>
#include <actl/random/random.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/types.hpp>
#include <iostream>

#define TEST_IMPL(name, va_args)                                    \
    namespace {                                                     \
    struct name : ac::tests::detail::test_base {                    \
        ac::czstring filename() const override { return __FILE__; } \
        int line() const override { return __LINE__; }              \
        ac::czstring args() const override { return va_args; }      \
        void body(ac::default_random&) const override;              \
        name() { ac::tests::detail::all_tests().push_back(this); }  \
        static name initializer;                                    \
    } name::initializer;                                            \
    }                                                               \
    inline void name::body([[maybe_unused]] ac::default_random& random) const

#define TEST(...) TEST_IMPL(CAT(_tesT_, __COUNTER__), #__VA_ARGS__)

#define ASSERT_EQUAL      ac::tests::detail::assert_impl(__FILE__, __LINE__).check<true>
#define ASSERT_NOT_EQUAL  ac::tests::detail::assert_impl(__FILE__, __LINE__).check<false>
#define ASSERT_TRUE       ac::tests::detail::assert_impl(__FILE__, __LINE__).check_true
#define ASSERT_FALSE      ac::tests::detail::assert_impl(__FILE__, __LINE__).check_false
#define ASSERT_EQUAL_SETS ac::tests::detail::assert_impl(__FILE__, __LINE__).check_sets
#define ASSERT_THROWS(expression) \
    ac::tests::detail::assert_throws(__FILE__, __LINE__, [&] { (void)(expression); })

namespace ac::tests {

namespace detail {

struct assert_impl {
    czstring filename;
    int line;

    assert_impl(czstring filename, int line) : filename{filename}, line{line} {}

    template <bool Equal, class T0, class T1>
    inline void check(const T0& expected, const T1& actual) const {
        if ((expected == actual) == Equal) return;
        std::stringstream ss;
        if (!Equal) ss << "not ";
        ss << "expected = " << diagnostics::to_string(expected)
           << ", actual = " << diagnostics::to_string(actual);
        ss << "; line = " << line;
        throw ss.str();
    }

    template <bool Equal, class T, class = std::enable_if_t<std::is_floating_point_v<T>>>
    inline void check(T expected, T actual, T eps) const {
        T numerator   = std::abs(expected - actual);
        T denominator = std::max(std::max(std::abs(expected), std::abs(actual)), T{1});
        if ((numerator <= eps * denominator) == Equal) return;
        std::stringstream ss;
        if (!Equal) ss << "not ";
        ss << "expected = " << expected << ", actual = " << actual
           << ", error = " << numerator / denominator;
        ss << "; line = " << line;
        throw ss.str();
    }

    inline void check_true(bool condition) const { check<true>(true, condition); }

    inline void check_false(bool condition) const { check<true>(false, condition); }

    template <class T>
    inline void check_sets(std::vector<T> expected, std::vector<T> actual) const {
        sort(expected);
        sort(actual);
        if (equal(expected, actual)) return;
        std::stringstream ss;
        ss << "expected = " << diagnostics::to_string(expected)
           << ", actual = " << diagnostics::to_string(actual);
        ss << "; line = " << line;
        throw ss.str();
    }
};

template <class Function>
inline void assert_throws(czstring, int line, const Function& f) {
    try {
        f();
    } catch (...) {
        return;
    }
    std::stringstream ss;
    ss << "expected exception";
    ss << "; line = " << line;
    throw ss.str();
}

struct test_base {
    virtual czstring filename() const = 0;
    virtual int line() const = 0;
    virtual czstring args() const = 0;
    virtual void body(default_random& random) const = 0;

    bool run();
};

std::vector<test_base*>& all_tests();

}  // namespace detail

int run(int argc, czstring argv[]);

}  // namespace ac::tests
