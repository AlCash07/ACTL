// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_traits.hpp>
#include <iterator>

namespace ac {

namespace detail {

template <class T, class = void>
struct is_iter : std::false_type {};

template <class T>
struct is_iter<T, void_t<typename std::iterator_traits<T>::iterator_category>>
    : std::true_type {};

} // namespace detail

template <class T, class = void>
struct is_iterator : detail::is_iter<T> {};

template <class T>
struct is_iterator<T*, std::enable_if_t<!std::is_object_v<T>>>
    : std::false_type {};

template <class T>
constexpr bool is_iterator_v = is_iterator<T>::value;

// TODO: implement is_output_iterator trait.

} // namespace ac
