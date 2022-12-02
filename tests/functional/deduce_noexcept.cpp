// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/deduce_noexcept.hpp>
#include <type_traits>

constexpr bool is_int(int) noexcept
{
    return true;
}

constexpr bool is_int(double x)
{
    if (x < 0)
        throw;
    return false;
}

/* AC_DEDUCE_NOEXCEPT_AND_RETURN */

template <class T>
constexpr auto is_int_wrapped(T x) AC_DEDUCE_NOEXCEPT_AND_RETURN(is_int(x))

static_assert(noexcept(is_int_wrapped(0)));
static_assert(is_int_wrapped(0));
static_assert(!noexcept(is_int_wrapped(-1.0)));
static_assert(!is_int_wrapped(0.0));

/* AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN */

template <class T>
constexpr auto is_int_wrapped2(T x)
    AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(is_int(x))

static_assert(noexcept(is_int_wrapped2(0)));
static_assert(is_int_wrapped2(0));
static_assert(!noexcept(is_int_wrapped2(-1.0)));
static_assert(!is_int_wrapped2(0.0));

template <class T>
concept has_is_int_wrapped2 = requires(T x)
{
    is_int_wrapped2(x);
};

static_assert(has_is_int_wrapped2<int>);
// This would be a compilation error with AC_DEDUCE_NOEXCEPT_AND_RETURN.
static_assert(!has_is_int_wrapped2<int[2]>);

/* AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN */

// Note that we can specify return type different from the wrapped function.
template <class T>
constexpr int is_int_wrapped3(T x)
    AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(is_int(x))

static_assert(noexcept(is_int_wrapped3(0)));
static_assert(std::is_same_v<int, decltype(is_int_wrapped3(0))>);
static_assert(0 != is_int_wrapped3(0));
static_assert(!noexcept(is_int_wrapped3(-1.0)));
static_assert(0 == is_int_wrapped3(0.0));

template <class T>
concept has_is_int_wrapped3 = requires(T x)
{
    is_int_wrapped3(x);
};

static_assert(has_is_int_wrapped3<int>);
// This would be a compilation error with AC_DEDUCE_NOEXCEPT_AND_RETURN.
static_assert(!has_is_int_wrapped3<int[2]>);

/* AC_DEDUCE_NOEXCEPT_AND_INITIALIZE */

struct Base
{
    bool value;

    template <class T>
    constexpr Base(T x) noexcept(noexcept(is_int(x))) : value{is_int(x)}
    {}
};

struct Derived : Base
{
    template <class T>
    constexpr Derived(T x) AC_DEDUCE_NOEXCEPT_AND_INITIALIZE(Base{x})
    {}
};

static_assert(noexcept(Derived{0}));
static_assert(Derived{0}.value);
static_assert(!noexcept(Derived{-1.0}));
static_assert(!Derived{0.0}.value);
