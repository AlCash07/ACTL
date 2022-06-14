// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/access/all.hpp>
#include <iterator>

namespace ac {

template <class T, class = void>
struct is_range : std::false_type
{};

template <class T> // & is needed for C arrays
struct is_range<
    T,
    std::void_t<decltype(
        ranges::begin(std::declval<T&>()), ranges::end(std::declval<T&>()))>>
    : std::true_type
{};

template <class T>
inline constexpr bool is_range_v = is_range<T>::value;

template <class T, class = void>
struct is_contiguous_range : std::false_type
{};

template <class T> // & is needed for C arrays
struct is_contiguous_range<
    T,
    std::void_t<decltype(
        ranges::data(std::declval<T&>()), ranges::size(std::declval<T&>()))>>
    : std::true_type
{};

template <class T>
inline constexpr bool is_contiguous_range_v = is_contiguous_range<T>::value;

} // namespace ac
