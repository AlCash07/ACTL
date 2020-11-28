/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/type_traits.hpp>
#include <iterator>

namespace ac {

template <class T, class = void>
struct is_range : std::false_type {};

template <class T>
struct is_range<T,
                std::void_t<decltype(std::begin(std::declval<T>()), std::end(std::declval<T>()))>>
    : std::true_type {};

template <class T>
constexpr bool is_range_v = is_range<T&>::value;  // & is needed for C arrays

template <class T, class = void>
struct is_contiguous_range : std::false_type {};

template <class T>
struct is_contiguous_range<
    T, std::void_t<decltype(std::data(std::declval<T>()), std::size(std::declval<T>()))>>
    : std::true_type {};

template <class T>
constexpr bool is_contiguous_range_v = is_contiguous_range<T&>::value;

template <class T>
struct range_traits {};

template <class T>
struct range_traits<const T> : range_traits<T> {};

template <class T, class = void>
struct is_sorted_range : std::false_type {};

template <class T>
struct is_sorted_range<T, std::void_t<typename range_traits<T>::is_sorted>> : std::true_type {};

template <class T>
constexpr bool is_sorted_range_v = is_sorted_range<T>::value;

template <class T, class = void>
struct is_unique_range : std::false_type {};

template <class T>
struct is_unique_range<T, std::void_t<typename range_traits<T>::is_unique>> : std::true_type {};

template <class T>
constexpr bool is_unique_range_v = is_unique_range<T>::value;

}  // namespace ac
