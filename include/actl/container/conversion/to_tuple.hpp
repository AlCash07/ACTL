// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/convert_to.hpp>
#include <actl/meta/tuple.hpp>
#include <actl/range/traits/strict_range.hpp>

namespace ac {

namespace detail {

template<typename S, typename Target, typename... Args>
struct to_tuple_impl;

template<size_t... Is, typename Target, typename... Args>
struct to_tuple_impl<std::index_sequence<Is...>, Target, Args...> {
    static constexpr bool value =
        (... && can_convert_to_v<std::tuple_element_t<Is, Target>, Args>);

    static constexpr Target convert(Args&&... args
    ) AC_DEDUCE_NOEXCEPT_AND_RETURN(Target{
        convert_to<std::tuple_element_t<Is, Target>>(std::forward<Args>(args)
        )...
    })
};

template<typename Target, typename... Args>
using to_tuple = to_tuple_impl<tuple_indices_t<Target>, Target, Args...>;

template<typename Target, typename... Args>
static constexpr bool can_initialize_tuple() {
    // Avoid conflicts with from_tuple specialization.
    if constexpr (sizeof...(Args) == 1 && (... && Tuple<Args>))
        return false;
    // Arrays and tuples may allow to specify not all their elements but only
    // some of the first ones.
    // Our conversions intentionnally forbid this to prevent mistakes.
    if constexpr (Tuple<Target> && !StrictRange<Target>)
        if constexpr (std::tuple_size_v<Target> == sizeof...(Args))
            return to_tuple<Target, Args...>::value;
    return false;
}

} // namespace detail

template<typename Target, typename... Args>
    requires(detail::can_initialize_tuple<Target, Args...>())
struct conversion<Target, Args...> : detail::to_tuple<Target, Args...> {};

} // namespace ac
