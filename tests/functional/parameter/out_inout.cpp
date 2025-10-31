// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/parameter/inout.hpp>
#include <actl/functional/parameter/out.hpp>
#include "test.hpp"

namespace ac {

int add_ints(int l, int r) {
    return l + r;
}

int& add_ints(out<int&> output, int l, int r) {
    return *output = l + r;
}

int& add_ints(inout<int&> l, int r) {
    return *l += r;
}

TEST_CASE("Output parameter wrappers example") {
    int x = 2;
    int y = 3;
    SECTION("regular return value") {
        CHECK(5 == add_ints(x, y));
    }
    SECTION("output-only parameter") {
        int sum;
        CHECK(5 == add_ints(out{sum}, x, y));
        CHECK(5 == sum);
    }
    SECTION("input-output parameter") {
        CHECK(5 == add_ints(inout{x}, y));
        CHECK(5 == x);
    }
}

TEST_CASE("assignment") {
    int output{};
    /* out */ {
        CHECK(5 == *(out{output} = 5));
        CHECK(5 == output);
        static_assert(std::is_same_v<
                      out<int&>&,
                      decltype(out{std::declval<int&>()} = 5)>);
        // void* is just an arbitrary impompatible type
        static_assert(!std::is_assignable_v<out<int>, void*>);
    }
    /* inout */ {
        CHECK(3 == *(inout{output} = 3));
        CHECK(3 == output);
        static_assert(std::is_same_v<
                      inout<int&>&,
                      decltype(inout{std::declval<int&>()} = 3)>);
        static_assert(!std::is_assignable_v<inout<int>, void*>);
    }
}

struct TestPtr {
    // const is used here to be able to store a pointer to constexpr value.
    // Pointed to object isn't modified in the test anyway.
    const int* ptr;

    constexpr TestPtr(const int* x) noexcept : ptr{x} {}
    // not noexcept to have a non-noexcept constructor for testing
    constexpr TestPtr(const int& x) : ptr{&x} {}

    explicit constexpr TestPtr(const void* x) noexcept
        : ptr{static_cast<const int*>(x)} {}

    /// Won't compile if the argument is const.
    constexpr bool isNonConst() {
        return true;
    }
};

constexpr int intValue = 42;

template<template<typename> typename Wrapper>
constexpr bool test_wrapper_constructors() {
    /* Constructor noexcept specification follows the wrapped type */ {
        static_assert(noexcept(Wrapper<TestPtr>{std::declval<const int*>()}));
        static_assert(!noexcept(Wrapper<TestPtr>{std::declval<const int&>()}));
    }
    /* Nested wrappers are combined into a single one */ {
        static_assert(std::is_same_v<
                      Wrapper<int&>,
                      decltype(Wrapper{std::declval<Wrapper<int&>>()})>);
        static_assert(std::is_same_v<
                      Wrapper<int&>,
                      decltype(Wrapper{std::declval<Wrapper<int&>&>()})>);
    }
    /* Conversion into the wrapped type is implicit */ {
        static_assert(std::is_convertible_v<Wrapper<int&>, int&>);
        static_assert(&intValue == Wrapper<const int*>{&intValue});
    }
    return true;
}

template<template<typename> typename Wrapper>
constexpr bool test_wrapper_conversions() {
    /* Conversion is allowed only if the wrapped types are implicitly
     * convertible */
    {
        static_assert(std::is_convertible_v<
                      Wrapper<const int*>,
                      Wrapper<TestPtr>>);
        // we need a variable here, because ->ptr access isn't const.
        Wrapper<TestPtr> wrapped{Wrapper<const int*>{&intValue}};
        if (&intValue != wrapped->ptr)
            return false;
        static_assert(!std::is_constructible_v<
                      Wrapper<TestPtr>,
                      Wrapper<const void*>>);
    }
    /* Conversion noexcept specification follows the wrapped types */ {
        static_assert(std::is_nothrow_constructible_v<
                      Wrapper<TestPtr>,
                      Wrapper<const int*>>);
        static_assert(std::is_convertible_v<
                      Wrapper<const int&>,
                      Wrapper<TestPtr>>);
        static_assert(!std::is_nothrow_constructible_v<
                      Wrapper<TestPtr>,
                      Wrapper<const int&>>);
    }
    return true;
}

static_assert(test_wrapper_constructors<out>());
static_assert(test_wrapper_constructors<inout>());

static_assert(test_wrapper_conversions<out>());
static_assert(test_wrapper_conversions<inout>());

/* conversion works from inout to out, but not the other way */
static_assert(!std::is_constructible_v<inout<int&>, out<int&>>);
static_assert(std::is_convertible_v<inout<int&>, out<int&>>);
static_assert(&intValue == *out<const int*>{inout<const int*>{&intValue}});

/* member access (non-const) */
static_assert(out<TestPtr> { &intValue } -> isNonConst());
static_assert(inout<TestPtr> { &intValue } -> isNonConst());

/* detection traits */

static_assert(is_out_v<out<int&>>);
static_assert(is_out_v<inout<int&>>);
static_assert(!is_out_v<int&>);

static_assert(is_inout_v<inout<int&>>);
static_assert(!is_inout_v<int&>);
static_assert(!is_inout_v<out<int&>>);

} // namespace ac
