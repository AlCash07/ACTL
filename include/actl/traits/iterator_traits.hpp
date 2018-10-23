/***************************************************************************************************
 * Type traits to check if a type is an iterator (that don't rely on iterator_category in order to
 * accept C arrays), and to compare iterator category.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <iterator>
#include <type_traits>

namespace ac {

namespace detail {

template <class T, class = void>
struct is_iterator : std::false_type {};

template <class T>
struct is_iterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category>>
    : std::true_type {};

}  // namespace detail

template <class T>
inline constexpr bool is_iterator_v = detail::is_iterator<T>::value;

namespace detail {

template <class T>
struct is_const_pointer : std::false_type {};

template <class T>
struct is_const_pointer<const T*> : std::true_type {};

template <class T, bool = is_iterator_v<T>>
struct is_const_iterator : is_const_pointer<typename std::iterator_traits<T>::pointer> {};

template <class T>
struct is_const_iterator<T, false> : std::false_type {};

template <class T, class Category, bool = is_iterator_v<T>>
struct has_iterator_category
    : std::is_same<Category, typename std::iterator_traits<T>::iterator_category> {};

template <class T, class Category>
struct has_iterator_category<T, Category, false> : std::false_type {};

}  // namespace detail

template <class T>
inline constexpr bool is_const_iterator_v = detail::is_const_iterator<T>::value;

template <class T>
inline constexpr bool is_non_const_iterator_v = is_iterator_v<T> && !is_const_iterator_v<T>;

template <class T>
inline constexpr bool is_input_iterator_v =
    detail::has_iterator_category<T, std::input_iterator_tag>::value;

template <class T>
inline constexpr bool is_output_iterator_v =
    detail::has_iterator_category<T, std::output_iterator_tag>::value;

template <class T>
inline constexpr bool is_forward_iterator_v =
    detail::has_iterator_category<T, std::forward_iterator_tag>::value;

template <class T>
inline constexpr bool is_bidirectional_iterator_v =
    detail::has_iterator_category<T, std::bidirectional_iterator_tag>::value;

template <class T>
inline constexpr bool is_random_access_iterator_v =
    detail::has_iterator_category<T, std::random_access_iterator_tag>::value;

template <class T, class = void>
struct is_range : std::false_type {};

template <class T, size_t N>
struct is_range<T[N], void> : std::true_type {};

template <class T>
struct is_range<T,
                std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
    : std::true_type {};

template <class T>
inline constexpr bool is_range_v = is_range<T>::value;

}  // namespace ac