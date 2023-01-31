// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/convert_to.hpp>
#include <actl/meta/tuple.hpp>
#include <actl/range/traits/is_strict_range.hpp>

namespace ac {

namespace detail {

template <class S, class To, class... Args>
struct to_tuple_impl;

template <size_t... Is, class To, class... Args>
struct to_tuple_impl<std::index_sequence<Is...>, To, Args...>
{
    static constexpr bool value =
        (... && can_convert_to_v<std::tuple_element_t<Is, To>, Args>);

    static constexpr To convert(Args&&... xs) AC_DEDUCE_NOEXCEPT_AND_RETURN(To{
        convert_to<std::tuple_element_t<Is, To>>(std::forward<Args>(xs))...})
};

template <class To, class... Args>
using to_tuple =
    to_tuple_impl<std::make_index_sequence<std::tuple_size_v<To>>, To, Args...>;

template <class To, class... Args>
static constexpr bool can_initialize_tuple()
{
    // Avoid conflicts with from_tuple specialization.
    if constexpr (sizeof...(Args) == 1 && (... && Tuple<Args>))
        return false;
    // Arrays and tuples may allow to specify not all their elements but only
    // some of the first ones.
    // Our conversions intentionnally forbid this to prevent mistakes.
    if constexpr (Tuple<To> && !is_strict_range_v<To>)
        if constexpr (std::tuple_size_v<To> == sizeof...(Args))
            return to_tuple<To, Args...>::value;
    return false;
}

} // namespace detail

template <class To, class... Args>
    requires(detail::can_initialize_tuple<To, Args...>())
struct conversion<To, Args...> : detail::to_tuple<To, Args...>
{};

} // namespace ac
