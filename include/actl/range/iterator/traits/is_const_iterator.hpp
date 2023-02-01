// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/iterator/traits/category.hpp>

namespace ac {

namespace detail {

template <class T>
struct is_const_pointer : std::false_type
{};

template <class T>
struct is_const_pointer<T const*> : std::true_type
{};

template <class T, bool = std::input_or_output_iterator<T>>
struct is_const_iterator
    : is_const_pointer<typename std::iterator_traits<T>::pointer>
{};

template <class T>
struct is_const_iterator<T, false> : std::false_type
{};

} // namespace detail

template <class T>
inline constexpr bool is_const_iterator_v = detail::is_const_iterator<T>::value;

template <class T>
inline constexpr bool is_non_const_iterator_v =
    std::input_or_output_iterator<T> && !is_const_iterator_v<T>;

} // namespace ac
