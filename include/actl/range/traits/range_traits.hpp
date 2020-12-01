/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/range/traits/is_range.hpp>
#include <type_traits>

namespace ac {

struct default_range_traits {
    static constexpr bool is_simple_associative = false;
    static constexpr bool is_pair_associative = false;
    static constexpr bool is_container = false;
    static constexpr bool is_sorted = false;
    static constexpr bool is_unique = false;
};

template <class T>
struct range_traits : default_range_traits {};

template <class T>
struct range_traits<const T> : range_traits<T> {};

template <class T, size_t N>
struct range_traits<T[N]> : default_range_traits {
    static constexpr bool is_container = true;
};

template <class T>
constexpr bool is_simple_associative_range_v = range_traits<T>::is_simple_associative;

template <class T>
constexpr bool is_pair_associative_range_v = range_traits<T>::is_pair_associative;

template <class T>
constexpr bool is_associative_range_v =
    is_simple_associative_range_v<T> || is_pair_associative_range_v<T>;

template <class C>
constexpr bool is_sequence_range_v = is_range_v<C> && !is_associative_range_v<C>;

template <class T>
constexpr bool is_container_v = range_traits<T>::is_container;

template <class T>
constexpr bool is_sorted_range_v = range_traits<T>::is_sorted;

template <class T>
constexpr bool is_unique_range_v = range_traits<T>::is_unique;

}  // namespace ac
