/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/cp/diagnostics/to_string.hpp>
#include <actl/io/all.hpp>
#include <actl/macros.hpp>
#include <actl/numeric/random/random.hpp>
#include <actl/range/algorithm.hpp>

using namespace std::string_literals;
using namespace ac;

#define TEST_IMPL(name, va_args)                                \
    namespace {                                                 \
    struct name : tests::detail::test_base {                    \
        czstring filename() const override { return __FILE__; } \
        int line() const override { return __LINE__; }          \
        czstring args() const override { return va_args; }      \
        void body(default_random&) const override;              \
        name() { tests::detail::all_tests().push_back(this); }  \
        static name initializer;                                \
    } name::initializer;                                        \
    }                                                           \
    inline void name::body([[maybe_unused]] default_random& random) const

#define TEST(...) TEST_IMPL(CAT(_tesT_, __COUNTER__), #__VA_ARGS__)

#define ASSERT_EQUAL        tests::detail::assert_impl(__FILE__, __LINE__).check
#define ASSERT_TRUE         tests::detail::assert_impl(__FILE__, __LINE__).check_true
#define ASSERT_FALSE        tests::detail::assert_impl(__FILE__, __LINE__).check_false
#define ASSERT_EQUAL_RANGES tests::detail::assert_impl(__FILE__, __LINE__).check_ranges
#define ASSERT_EQUAL_SETS   tests::detail::assert_impl(__FILE__, __LINE__).check_sets
#define ASSERT_THROWS(expression) \
    tests::detail::assert_throws(__FILE__, __LINE__, [&] { (void)(expression); })

namespace ac::io {

template <class Device, class Format, class T>
inline std::enable_if_t<!is_range_v<T> && !is_composite<T>::value, index>
    serialize(Device& od, Format&, const T& x) {
    return od.write(char_span{"<unknown-type>"});
}

}  // namespace ac::io

namespace ac::tests {

namespace detail {

template <class T>
inline std::string to_string(const T& value) {
    std::string s;
    io::spaced<io::pretty<io::text_static<bit(io::flags::boolalpha)>>> fmt;
    io::write(io::string<io::app>{s}, fmt, value);
    return s;
}

struct assert_impl {
    czstring filename;
    int line;

    assert_impl(czstring filename, int line) : filename{filename}, line{line} {}

    template <class T0, class T1>
    inline void check(const T0& expected, const T1& actual) const {
        if (expected == actual) return;
        throw "expected = " + to_string(expected) +
            "\nactual   = " + to_string(expected) +
            "\nline = " + to_string(line);
    }

    template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>>
    inline void check(T expected, T actual, T eps) const {
        T numerator   = std::abs(expected - actual);
        T denominator = std::max(std::max(std::abs(expected), std::abs(actual)), T{1});
        if (numerator <= eps * denominator) return;
        throw "expected = " + to_string(expected) +
            "\nactual   = " + to_string(expected) +
            "\nerror    = " + to_string(numerator / denominator) +
            "\nline = " + to_string(line);
    }

    inline void check_true(bool condition) const { check(true, condition); }

    inline void check_false(bool condition) const { check(false, condition); }

    template <class T0, class T1>
    inline void check_ranges(const T0& expected, const T1& actual) const {
        if (equal(expected, actual)) return;
        throw "expected = " + to_string(expected) +
            "\nactual   = " + to_string(expected) +
            "\nline = " + to_string(line);
    }

    template <class T>
    inline void check_sets(std::vector<T> expected, std::vector<T> actual) const {
        sort(expected);
        sort(actual);
        check_ranges(expected, actual);
    }
};

template <class Function>
inline void assert_throws(czstring, int line, const Function& f) {
    try {
        f();
    } catch (...) {
        return;
    }
    throw "expected exception"
        "\nline = " + to_string(line);
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
