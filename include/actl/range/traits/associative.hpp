/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/range/traits/is_range.hpp>
#include <actl/range/traits/range_traits.hpp>

namespace ac {

template <class T, class = void>
struct is_simple_associative_range : std::false_type {};

template <class T>
struct is_simple_associative_range<T, std::void_t<typename range_traits<T>::is_simple_associative>>
    : std::true_type {};

template <class T>
constexpr bool is_simple_associative_range_v = is_simple_associative_range<T>::value;

template <class T, class = void>
struct is_pair_associative_range : std::false_type {};

template <class T>
struct is_pair_associative_range<T, std::void_t<typename range_traits<T>::is_pair_associative>>
    : std::true_type {};

template <class T>
constexpr bool is_pair_associative_range_v = is_pair_associative_range<T>::value;

template <class T>
constexpr bool is_associative_range_v =
    is_simple_associative_range_v<T> || is_pair_associative_range_v<T>;

template <class C>
constexpr bool is_sequence_range_v = is_range_v<C> && !is_associative_range_v<C>;

}  // namespace ac
