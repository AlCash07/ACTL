/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/traits.hpp>
#include <actl/range/traits/is_range.hpp>

namespace ac {

template <class T, class Category, bool = is_range_v<T>>
struct has_range_iterator : std::is_base_of<Category, iterator_category_t<iterator_t<T>>> {};

template <class T, class Category>
struct has_range_iterator<T, Category, false> : std::false_type {};

template <class C>
constexpr bool is_random_access_range_v =
    has_range_iterator<C, std::random_access_iterator_tag>::value;

template <class T, class = void>
struct is_contiguous_range : std::false_type {};

template <class T>
struct is_contiguous_range<
    T, std::void_t<decltype(std::data(std::declval<T>()), std::size(std::declval<T>()))>>
    : std::true_type {};

template <class T>
constexpr bool is_contiguous_range_v = is_contiguous_range<T&>::value;

}  // namespace ac
