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

constexpr bool is_int(double)
{
    return false;
}

/* AC_DEDUCE_NOEXCEPT_AND_RETURN */

template <class T>
constexpr auto is_int_wrapped(T x) AC_DEDUCE_NOEXCEPT_AND_RETURN(is_int(x))

static_assert(noexcept(is_int_wrapped(0)));
static_assert(is_int_wrapped(0));
static_assert(!noexcept(is_int_wrapped(0.0)));
static_assert(!is_int_wrapped(0.0));

/* AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN */

template <class T>
constexpr auto is_int_wrapped2(T x)
    AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(is_int(x))

static_assert(noexcept(is_int_wrapped2(0)));
static_assert(is_int_wrapped2(0));
static_assert(!noexcept(is_int_wrapped2(0.0)));
static_assert(!is_int_wrapped2(0.0));

template <class T, class = void>
struct has_is_int_wrapped : std::false_type
{};

template <class T>
struct has_is_int_wrapped<
    T,
    std::void_t<decltype(is_int_wrapped2(std::declval<T>()))>> : std::true_type
{};

static_assert(has_is_int_wrapped<int>::value);
// This would be a compilation error with AC_DEDUCE_NOEXCEPT_AND_RETURN.
static_assert(!has_is_int_wrapped<int[2]>::value);

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
static_assert(!noexcept(Derived{0.0}));
static_assert(!Derived{0.0}.value);
