/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/macros.hpp>
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

#define is_XXX_iterator_v(name)                              \
    template <class T>                                       \
    inline constexpr bool CAT(CAT(is_, name), _iterator_v) = \
        detail::has_iterator_category<T, std::CAT(name, _iterator_tag)>::value

is_XXX_iterator_v(input);
is_XXX_iterator_v(output);
is_XXX_iterator_v(forward);
is_XXX_iterator_v(bidirectional);
is_XXX_iterator_v(random_access);

#undef is_XXX_iterator_v

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
