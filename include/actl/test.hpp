// Copyright 2018 Roman Rizvanov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/composite/all.hpp>
#include <actl/functional/scalar/all.hpp>
#include <actl/io/all.hpp>
#include <actl/macros.hpp>
#include <actl/numeric/random/random.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/range/irange.hpp>
#include <string_view>

namespace ac {

inline io::formatted<io::file<io::out>, io::composed_format<io::spaced<char>, io::text>> cout{
    stdout};

}  // namespace ac

using namespace std::string_view_literals;
using namespace ac;

#define TEST_IMPL(name, va_args)                        \
    namespace {                                         \
    struct name : tests::detail::test_base {            \
        std::string_view filename() const override {    \
            return __FILE__;                            \
        }                                               \
        int line_number() const override {              \
            return __LINE__;                            \
        }                                               \
        std::string_view args() const override {        \
            return va_args;                             \
        }                                               \
        void body(default_random&) const override;      \
        name() {                                        \
            tests::detail::all_tests().push_back(this); \
        }                                               \
        static name initializer;                        \
    } name::initializer;                                \
    }                                                   \
    void name::body([[maybe_unused]] default_random& random) const

#define TEST(...) TEST_IMPL(CAT(_tesT_, __COUNTER__), #__VA_ARGS__)

#define ASSERT_EQUAL      tests::detail::assert_impl(__FILE__, __LINE__).check_equal
#define ASSERT_NOT_EQUAL  tests::detail::assert_impl(__FILE__, __LINE__).check_not_equal
#define ASSERT_TRUE       tests::detail::assert_impl(__FILE__, __LINE__).check_true
#define ASSERT_FALSE      tests::detail::assert_impl(__FILE__, __LINE__).check_false
#define ASSERT_EQUAL_SETS tests::detail::assert_impl(__FILE__, __LINE__).check_sets
#define ASSERT_THROWS(expression) \
    tests::detail::assert_throws(__FILE__, __LINE__, [&] { (void)(expression); })

namespace ac::io {

template <
    class Device, class Format, class T,
    enable_int_if<!(decltype(serialization_access{}.has_write<T, Device&, Format&>(0))::value ||
                    is_range_v<T> || is_io_tuple_v<T> || std::is_empty_v<T> ||
                    std::is_arithmetic_v<T>)> = 0>
index write_final(Device& od, Format& fmt, const T&) {
    return write(od, fmt, "<unknown-type>"sv);
}

}  // namespace ac::io

namespace ac::math {

template <class T>
struct abs_rel_error : scalar_operation<abs_rel_error<T>, 2, scalar_tag> {
    struct is_policy;

    abs_rel_error(T eps) : eps{eps} {}

    T eps;

    bool eval_scalar(T lhs, T rhs) const {
        T numerator = abs(lhs - rhs);
        T denominator = max(max(abs(lhs), abs(rhs)), T{1});
        return numerator <= eps * denominator;
    }
};

template <class T>
auto apply_policy(Equal, const abs_rel_error<T>& policy) {
    return policy;
}

}  // namespace ac::math

namespace ac::tests {

namespace detail {

template <class T>
std::string to_string(const T& value) {
    std::string s;
    auto fmt = io::spaced{", "sv} >>= io::escaped{} >>= io::braced{} >>=
        io::text_static<bit(io::flags::boolalpha)>{};
    io::write(io::string<io::app>{s}, fmt, value);
    return s;
}

enum message_kind : uint8_t { Expected, NotExpected, Actual, Line };

template <message_kind Kind, class T>
std::string message(const T& x) {
    static std::string messages[4] = {"expected = ", "expected = not ", "actual   = ", "line = "};
    return messages[Kind] + to_string(x) + "\n";
}

struct assert_impl {
    std::string_view filename;
    int line;

    assert_impl(std::string_view filename, int line) : filename{filename}, line{line} {}

    template <class T, class U>
    void check_equal(const T& expected, const U& actual) const {
        if (eval(math::equal(expected, actual))) return;
        throw message<Expected>(expected) + message<Actual>(actual) + message<Line>(line);
    }

    template <class T, class U>
    void check_not_equal(const T& not_expected, const U& actual) const {
        if (!eval(math::equal(not_expected, actual))) return;
        throw message<NotExpected>(not_expected) + message<Actual>(actual) + message<Line>(line);
    }

    template <class T, class U, class E>
    void check_equal(const T& expected, const U& actual, E eps) const {
        if (eval(math::equal(math::abs_rel_error<E>{eps})(expected, actual))) return;
        throw message<Expected>(expected) + message<Actual>(actual) + message<Line>(line);
    }

    template <class T>
    void check_true(const T& condition) const {
        check_equal(true, condition);
    }

    template <class T>
    void check_false(const T& condition) const {
        check_equal(false, condition);
    }

    template <class T>
    void check_sets(std::vector<T> expected, std::vector<T> actual) const {
        sort(expected);
        sort(actual);
        check_equal(expected, actual);
    }
};

template <class Function>
void assert_throws(std::string_view, int line, const Function& f) {
    try {
        f();
    } catch (...) {
        return;
    }
    throw "expected exception\n" + message<Line>(line);
}

struct test_base {
    virtual std::string_view filename() const = 0;
    virtual int line_number() const = 0;
    virtual std::string_view args() const = 0;
    virtual void body(default_random& random) const = 0;

    bool run();
};

std::vector<test_base*>& all_tests();

}  // namespace detail

int run(int argc, const char* argv[]);

}  // namespace ac::tests
