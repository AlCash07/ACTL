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
        CHECK(5 == (out{output} = 5));
        CHECK(5 == output);
        static_assert(std::is_same_v<
                      int&,
                      decltype(out{std::declval<int&>()} = 5)>);
        // void* is just an arbitrary incompatible type
        static_assert(!std::is_assignable_v<out<int&>, void*>);
    }
    /* inout */ {
        CHECK(3 == (inout{output} = 3));
        CHECK(3 == output);
        static_assert(std::is_same_v<
                      int&,
                      decltype(inout{std::declval<int&>()} = 3)>);
        static_assert(!std::is_assignable_v<inout<int&>, void*>);
    }
}

struct wrapped_int {
    int x;
};

struct test_ref {
    // const is used here to be able to reference a constexpr value.
    // Referenced object isn't modified in the test anyway.
    int const& ref;

    constexpr test_ref(int const& x) noexcept : ref{x} {}
    // This constructor doesn't make much sense,
    // we just need a non-noexcept constructor for testing.
    constexpr test_ref(int& x) : ref{x} {}
    // This constructor doesn't make much sense,
    // we just need an explicit constructor for testing.
    explicit constexpr test_ref(wrapped_int& w) : ref{w.x} {}

    /// Won't compile if the argument is const.
    constexpr bool is_non_const() {
        return true;
    }
};

template<>
struct is_reference<test_ref> : std::true_type {};

constexpr int intValue = 42;

template<template<typename> typename Wrapper>
constexpr bool test_wrapper_constructors() {
    /* Constructor noexcept specification follows the wrapped type */ {
        static_assert(noexcept(Wrapper<test_ref>{std::declval<int const&>()}));
        static_assert(!noexcept(Wrapper<test_ref>{std::declval<int&>()}));
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
        static_assert(std::is_convertible_v<Wrapper<int const&>, int const&>);
        static_assert(&intValue == &*Wrapper<int const&>{intValue});
    }
    return true;
}

template<template<typename> typename Wrapper>
constexpr bool test_wrapper_conversions() {
    /* Conversion is allowed only if the wrapped types are implicitly
     * convertible */
    {
        static_assert(std::is_convertible_v<
                      Wrapper<int const&>,
                      Wrapper<test_ref>>);
        // we need a variable here, because ->ref access isn't const.
        Wrapper<test_ref> wrapped{Wrapper<int const&>{intValue}};
        if (&intValue != &wrapped->ref)
            return false;
        static_assert(!std::is_constructible_v<
                      Wrapper<test_ref>,
                      Wrapper<wrapped_int&>>);
    }
    /* Conversion noexcept specification follows the wrapped types */ {
        static_assert(std::is_nothrow_constructible_v<
                      Wrapper<test_ref>,
                      Wrapper<int const&>>);
        static_assert(std::is_convertible_v<
                      Wrapper<int const&>,
                      Wrapper<test_ref>>);
        static_assert(!std::is_nothrow_constructible_v<
                      Wrapper<test_ref>,
                      Wrapper<int&>>);
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
static_assert(&intValue == &*out<int const&>{inout<int const&>{intValue}});

/* member access (non-const) */
static_assert(out<test_ref>(intValue)->is_non_const());
static_assert(inout<test_ref>(intValue)->is_non_const());

/* detection traits */

static_assert(is_out_v<out<int&>>);
static_assert(is_out_v<inout<int&>>);
static_assert(!is_out_v<int&>);

static_assert(is_inout_v<inout<int&>>);
static_assert(!is_inout_v<int&>);
static_assert(!is_inout_v<out<int&>>);

} // namespace ac
