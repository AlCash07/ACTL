// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/range.hpp>
#include <type_traits>

namespace ac {

struct default_range_properties
{
    static constexpr bool is_container = false;
    static constexpr bool is_simple_associative = false;
    static constexpr bool is_pair_associative = false;
    static constexpr bool is_sorted = false;
    static constexpr bool is_unique = false;
};

template <class T>
struct range_properties : default_range_properties
{};

template <class T>
struct range_properties<T const> : range_properties<T>
{};

template <class T, size_t N>
struct range_properties<T[N]> : default_range_properties
{
    static constexpr bool is_container = true;
};

template <class T>
inline constexpr bool is_container_v = range_properties<T>::is_container;

template <class T>
inline constexpr bool is_simple_associative_range_v =
    range_properties<T>::is_simple_associative;

template <class T>
inline constexpr bool is_pair_associative_range_v =
    range_properties<T>::is_pair_associative;

template <class T>
inline constexpr bool is_associative_range_v =
    is_simple_associative_range_v<T> || is_pair_associative_range_v<T>;

template <class C>
inline constexpr bool is_sequence_range_v =
    is_range_v<C> && !is_associative_range_v<C>;

template <class T>
inline constexpr bool is_sorted_range_v = range_properties<T>::is_sorted;

template <class T>
inline constexpr bool is_unique_range_v = range_properties<T>::is_unique;

} // namespace ac
