// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/concepts.hpp>
#include <actl/range/traits/super_range.hpp>
#include <type_traits>

namespace ac {

namespace detail {

template <class T, class = void>
struct has_key_type : std::false_type
{};

template <class T>
struct has_key_type<T, std::void_t<typename T::key_type>> : std::true_type
{};

template <class T>
struct has_key_type<T, std::enable_if_t<has_super_range_v<T>>>
    : has_key_type<super_range_t<T>>
{};

template <class T, class = void>
struct has_mapped_type : std::false_type
{};

template <class T>
struct has_mapped_type<T, std::void_t<typename T::mapped_type>> : std::true_type
{};

template <class T>
struct has_mapped_type<T, std::enable_if_t<has_super_range_v<T>>>
    : has_mapped_type<super_range_t<T>>
{};

} // namespace detail

template <class T>
inline constexpr bool is_associative_range_v =
    Range<T> && detail::has_key_type<std::remove_reference_t<T>>::value;

template <class T>
inline constexpr bool is_pair_associative_range_v =
    is_associative_range_v<T> &&
    detail::has_mapped_type<std::remove_reference_t<T>>::value;

template <class T>
inline constexpr bool is_simple_associative_range_v =
    is_associative_range_v<T> && !is_pair_associative_range_v<T>;

template <class C>
inline constexpr bool is_sequence_range_v =
    Range<C> && !is_associative_range_v<C>;

} // namespace ac
