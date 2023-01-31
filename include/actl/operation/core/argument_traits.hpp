// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/associated_types.hpp>

namespace ac {

template <class T>
struct raw
{
    using type = T;
};

template <class T>
using raw_t = typename raw<std::remove_cvref_t<T>>::type;

namespace detail {

template <bool B, class T>
struct value_if
{
    using type = range_value_t<raw_t<T>>;
};

template <class T>
struct value_if<false, T>
{
    using type = T;
};

template <bool B, class T>
using value_type_if_t = typename value_if<B, T>::type;

} // namespace detail

template <class T, class U = std::remove_cvref_t<T>>
using value_if_small =
    std::conditional_t<std::is_empty_v<U> || std::is_arithmetic_v<U>, U, T>;

// pass is the same as std::forward except it converts reference into const
// reference
template <class T>
constexpr T&& pass(std::remove_reference_t<T>& x)
{
    return static_cast<T&&>(x);
}

template <class T>
constexpr T const&& pass(std::remove_reference_t<T>&& x)
{
    static_assert(
        !std::is_lvalue_reference_v<T>, "can not pass an rvalue as an lvalue");
    return static_cast<T const&&>(x);
}

} // namespace ac
