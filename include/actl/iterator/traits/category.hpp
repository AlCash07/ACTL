// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/traits/is_iterator.hpp>

namespace ac {

template <class T>
using iterator_category_t = typename std::iterator_traits<T>::iterator_category;

namespace detail {

template <class T, class Category, bool = is_iterator_v<T>>
struct has_iterator_category : std::is_base_of<Category, iterator_category_t<T>>
{};

template <class T, class Category>
struct has_iterator_category<T, Category, false> : std::false_type
{};

} // namespace detail

template <class T>
constexpr bool is_input_iterator_v =
    detail::has_iterator_category<T, std::input_iterator_tag>::value;

template <class T>
constexpr bool is_forward_iterator_v =
    detail::has_iterator_category<T, std::forward_iterator_tag>::value;

template <class T>
constexpr bool is_bidirectional_iterator_v =
    detail::has_iterator_category<T, std::bidirectional_iterator_tag>::value;

template <class T>
constexpr bool is_random_access_iterator_v =
    detail::has_iterator_category<T, std::random_access_iterator_tag>::value;

// TODO: implement is_output_iterator trait.

} // namespace ac
