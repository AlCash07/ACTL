// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/static_size.hpp>
#include <type_traits>
#include <utility>

namespace ac {

namespace detail {

template <class T, class Seq, class = void>
struct has_constant_indices : std::false_type
{};

template <class T, size_t... Is>
struct has_constant_indices<
    T,
    std::index_sequence<Is...>,
    std::void_t<decltype(std::declval<T>()[ac::constant<Is>{}])...>>
    : std::true_type
{};

template <class T, class = void>
struct has_dynamic_index : std::false_type
{};

template <class T>
struct has_dynamic_index<
    T,
    std::void_t<
        decltype(std::declval<T>().size()),
        decltype(std::declval<T>()[size_t{}])>> : std::true_type
{};

} // namespace detail

/// We define the "array" concept as something with size and subscript operator
/// to index the elements from 0 to size-1.
/// Elements don't have to be of the same, otherwise this would be just a random
/// access range. A tuple could also fit this concept if it implemented
/// subscript operator taking `ac::constant`.
/// Names like "sequence" don't fit here, because types like linked list are
/// also sequences.
template <class T, size_t Size = ac::static_size_v<T>, class = void>
struct is_array
    : detail::has_constant_indices<T, std::make_index_sequence<Size>>
{};

template <class T>
struct is_array<T, dynamic_size> : detail::has_dynamic_index<T>
{};

template <class T>
inline constexpr bool is_array_v = is_array<T>::value;

} // namespace ac
