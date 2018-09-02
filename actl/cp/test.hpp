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
#include <iostream>

#define TEST_IMPL(cn, as)                                          \
    namespace {                                                    \
    struct cn : ac::tests::detail::test_base {                     \
        const char* filename() const override { return __FILE__; } \
        int         line() const override { return __LINE__; }     \
        const char* args() const override { return as; }           \
        void        body(ac::default_random&) const override;      \
        cn() { ac::tests::detail::all_tests().push_back(this); }   \
        static cn initializer;                                     \
    } cn::initializer;                                             \
    }                                                              \
    inline void cn::body(ac::default_random& random) const

#define TEST(...)        TEST_IMPL(CONCATENATE(_tesT_, __COUNTER__), #__VA_ARGS__)

#define ASSERT_EQUAL     ac::tests::detail::assert_impl(__FILE__, __LINE__).check<true>
#define ASSERT_NOT_EQUAL ac::tests::detail::assert_impl(__FILE__, __LINE__).check<false>
#define ASSERT_TRUE      ac::tests::detail::assert_impl(__FILE__, __LINE__).check_true
#define ASSERT_FALSE     ac::tests::detail::assert_impl(__FILE__, __LINE__).check_false
#define ASSERT_THROWS(expression) \
    ac::tests::detail::assert_throws(__FILE__, __LINE__, [&] { (void)(expression); })

namespace ac::tests {

namespace detail {

struct assert_impl {
    const char* filename;
    int line;
    assert_impl(const char* filename, int line) : filename(filename), line(line) {}

    template <bool Equal, class T1, class T2>
    inline void check(const T1& expected, const T2& actual) const {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
        if ((expected == actual) == Equal) return;
#pragma clang diagnostic pop
        std::stringstream ss;
        if (!Equal) ss << "not ";
        ss << "expected = " << diagnostics::to_string(expected)
           << ", actual = " << diagnostics::to_string(actual);
        ss << "; line = " << line;
        throw ss.str();
    }

    template <bool Equal>
    inline void check(double expected, double actual, double eps) const {
        double num = std::abs(expected - actual);
        double denom = std::max(std::max(std::abs(expected), std::abs(actual)), 1.0);
        if ((num <= eps * denom) == Equal) return;
        std::stringstream ss;
        if (!Equal) ss << "not ";
        ss << "expected = " << expected << ", actual = " << actual << ", error = " << num / denom;
        ss << "; line = " << line;
        throw ss.str();
    }

    inline void check_true(bool condition) const { check<true>(true, condition); }

    inline void check_false(bool condition) const { check<true>(false, condition); }
};

template <class F>
inline void assert_throws(const char* filename, int line, const F& f) {
    bool ok = true;
    try { f(); ok = false; } catch(...) {}
    if (ok) return;
    std::stringstream ss;
    ss << "expected exception";
    ss << "; line = " << line;
    throw ss.str();
}

struct test_base {
    virtual const char* filename() const = 0;
    virtual int line() const = 0;
    virtual const char* args() const = 0;
    virtual void body(default_random& random) const = 0;
    
    bool run();
};

std::vector<test_base*>& all_tests();

}  // namespace detail

int run(int argc, const char* argv[]);

}  // namespace ac::tests
