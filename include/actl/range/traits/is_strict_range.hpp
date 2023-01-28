// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/tuple.hpp>
#include <actl/meta/type_traits.hpp>
#include <actl/range/traits/associated_types.hpp>

namespace ac {

namespace detail {

template <class T, class Seq>
struct is_strict;

template <class T, size_t... Is>
struct is_strict<T, std::index_sequence<Is...>>
{
    static constexpr bool value =
        (... && std::is_same_v<range_value_t<T>, std::tuple_element_t<Is, T>>);
};

} // namespace detail

template <class T, bool = Range<T>, bool = Tuple<T>>
struct is_strict_range : std::false_type
{};

template <class T>
struct is_strict_range<T, true, false> : std::true_type
{};

template <class T>
struct is_strict_range<T, true, true>
    : detail::is_strict<T, std::make_index_sequence<std::tuple_size_v<T>>>
{};

/// Checks that we can't deduce stricter types for the range elements if we view
/// it as a tuple. That is, if the range @p T is also a tuple, then all the
/// elements of this tuple are the same as range_value_t<T>.
template <class T>
inline constexpr bool is_strict_range_v =
    is_strict_range<std::remove_cvref_t<T>>::value;

} // namespace ac
