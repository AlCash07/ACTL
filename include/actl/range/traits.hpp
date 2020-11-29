/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/type_traits.hpp>
#include <actl/iterator/traits.hpp>

namespace ac {

template <class T, class = void>
struct is_range : std::false_type {};

template <class T>
struct is_range<T,
                std::void_t<decltype(std::begin(std::declval<T>()), std::end(std::declval<T>()))>>
    : std::true_type {};

template <class T>
constexpr bool is_range_v = is_range<T&>::value;  // & is needed for C arrays

template <class T, class Category, bool = is_range_v<T>>
struct has_range_iterator : std::is_base_of<Category, iterator_category_t<iterator_t<T>>> {};

template <class T, class Category>
struct has_range_iterator<T, Category, false> : std::false_type {};

template <class C>
constexpr bool is_random_access_range_v = has_range_iterator<C, std::random_access_iterator_tag>::value;

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

template <class T, size_t N>
struct range_traits<T[N]> {
    struct is_container;
};

template <class T, class = void>
struct is_container : std::false_type {};

template <class T>
struct is_container<T, std::void_t<typename range_traits<T>::is_container>> : std::true_type {};

template <class T>
constexpr bool is_container_v = is_container<T>::value;

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
