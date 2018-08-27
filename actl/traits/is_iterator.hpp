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

template <class T>
using dereference_t = decltype(*std::declval<T>());

namespace detail {

template <class T, class = void>
struct is_iterator_impl : std::false_type {};

template <class T>
struct is_iterator_impl<T, std::void_t<dereference_t<T>>> : std::true_type {};

template <class T, bool = false>
struct is_const_iterator_impl : std::false_type {};

template <class T>
struct is_const_iterator_impl<T, true> : std::is_const<std::remove_reference_t<dereference_t<T>>> {
};

template <class T, class = void>
struct has_iterator_category_impl : std::false_type {};

template <class T>
struct has_iterator_category_impl<T, std::void_t<typename T::iterator_category>> : std::true_type {
};

}  // namespace detail

template <class T>
struct is_iterator : detail::is_iterator_impl<T> {};

template <class T>
struct is_const_iterator : detail::is_const_iterator_impl<T, is_iterator<T>::value> {};

template <class T>
struct is_non_const_iterator
    : std::bool_constant<is_iterator<T>::value && !is_const_iterator<T>::value> {};

template <class T>
struct has_iterator_category : detail::has_iterator_category_impl<std::iterator_traits<T>> {};

namespace detail {

template <class T, class Category, bool = ac::has_iterator_category<T>::value>
struct is_xxx_iterator
    : std::is_same<Category, typename std::iterator_traits<T>::iterator_category> {};

template <class T, class Category>
struct is_xxx_iterator<T, Category, false> : std::false_type {};

}  // namespace detail

template <class T>
struct is_input_iterator : detail::is_xxx_iterator<T, std::input_iterator_tag> {};

template <class T>
struct is_output_iterator : detail::is_xxx_iterator<T, std::output_iterator_tag> {};

template <class T>
struct is_forward_iterator : detail::is_xxx_iterator<T, std::forward_iterator_tag> {};

template <class T>
struct is_bidirectional_iterator : detail::is_xxx_iterator<T, std::bidirectional_iterator_tag> {};

template <class T>
struct is_random_access_iterator : detail::is_xxx_iterator<T, std::random_access_iterator_tag> {};

}  // namespace ac
