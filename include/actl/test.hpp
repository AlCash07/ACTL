/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/all.hpp>
#include <actl/macros.hpp>
#include <actl/numeric/random/random.hpp>
#include <actl/range/algorithm.hpp>
#include <string_view>

namespace ac {

inline io::formatted<io::file<io::out>, io::spaced<io::text>> cout{stdout};

}  // namespace ac

using namespace std::string_view_literals;
using namespace ac;

#define TEST_IMPL(name, va_args)                                        \
    namespace {                                                         \
    struct name : tests::detail::test_base {                            \
        std::string_view filename() const override { return __FILE__; } \
        int line() const override { return __LINE__; }                  \
        std::string_view args() const override { return va_args; }      \
        void body(default_random&) const override;                      \
        name() { tests::detail::all_tests().push_back(this); }          \
        static name initializer;                                        \
    } name::initializer;                                                \
    }                                                                   \
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

template <class Device, class Format, class T,
          std::enable_if_t<!is_range_v<T> && !is_composite<T>::value && !std::is_empty_v<T> &&
                           !std::is_same_v<T, io::char_t<Device>>>* = nullptr>
inline index serialize(Device& od, Format&, const T&) {
    return od.write(char_span{"<unknown-type>"});
}

}  // namespace ac::io

namespace ac::tests {

namespace detail {

template <class T>
inline std::string to_string(const T& value) {
    std::string s;
    io::spaced<io::pretty<io::text_static<bit(io::flags::boolalpha)>>> fmt;
    io::write(io::string<io::app>{s}, fmt, io::setspace{", "}, value);
    return s;
}

enum message_kind : uint8_t { Expected, Actual, Error, Line };

template <message_kind Kind, class T>
inline std::string message(const T& x) {
    static std::string messages[4] = {"expected = ", "actual   = ", "error    = ", "line = "};
    return messages[Kind] + to_string(x) + "\n";
}

struct assert_impl {
    std::string_view filename;
    int line;

    assert_impl(std::string_view filename, int line) : filename{filename}, line{line} {}

    template <class T0, class T1>
    inline void check(const T0& expected, const T1& actual) const {
        if (expected == actual) return;
        throw message<Expected>(expected) + message<Actual>(actual) + message<Line>(line);
    }

    template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>>
    inline void check(T expected, T actual, T eps) const {
        T numerator   = std::abs(expected - actual);
        T denominator = std::max(std::max(std::abs(expected), std::abs(actual)), T{1});
        if (numerator <= eps * denominator) return;
        throw message<Expected>(expected) + message<Actual>(actual) +
            message<Error>(numerator / denominator) + message<Line>(line);
    }

    inline void check_true(bool condition) const { check(true, condition); }

    inline void check_false(bool condition) const { check(false, condition); }

    template <class T0, class T1>
    inline void check_ranges(const T0& expected, const T1& actual) const {
        if (equal(expected, actual)) return;
        throw message<Expected>(expected) + message<Actual>(actual) + message<Line>(line);
    }

    template <class T>
    inline void check_sets(std::vector<T> expected, std::vector<T> actual) const {
        sort(expected);
        sort(actual);
        check_ranges(expected, actual);
    }
};

template <class Function>
inline void assert_throws(std::string_view, int line, const Function& f) {
    try {
        f();
    } catch (...) {
        return;
    }
    throw "expected exception\n" + message<Line>(line);
}

struct test_base {
    virtual std::string_view filename() const = 0;
    virtual int line() const = 0;
    virtual std::string_view args() const = 0;
    virtual void body(default_random& random) const = 0;

    bool run();
};

std::vector<test_base*>& all_tests();

}  // namespace detail

int run(int argc, const char* argv[]);

}  // namespace ac::tests
