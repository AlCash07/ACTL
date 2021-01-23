// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/scalar.hpp>
#include <actl/category/utility/is_subcategory_of.hpp>
#include <actl/meta/is_iterator.hpp>

namespace ac {

// clang-format off
struct iterator_tag               { using base = scalar_tag; };
struct input_iterator_tag         { using base = iterator_tag; };
struct forward_iterator_tag       { using base = input_iterator_tag; };
struct bidirectional_iterator_tag { using base = forward_iterator_tag; };
struct random_access_iterator_tag { using base = bidirectional_iterator_tag; };
// clang-format on

namespace detail {

template <class C>
struct iter_category {
    using type = iterator_tag;
};

template <>
struct iter_category<std::input_iterator_tag> {
    using type = input_iterator_tag;
};

template <>
struct iter_category<std::forward_iterator_tag> {
    using type = forward_iterator_tag;
};

template <>
struct iter_category<std::bidirectional_iterator_tag> {
    using type = bidirectional_iterator_tag;
};

template <>
struct iter_category<std::random_access_iterator_tag> {
    using type = random_access_iterator_tag;
};

}  // namespace detail

template <class T>
struct category_sfinae<T, std::enable_if_t<is_iterator_v<T>>>
    : detail::iter_category<typename std::iterator_traits<T>::iterator_category> {};

template <class T>
constexpr bool is_input_iterator_v = is_subcategory_of_v<category_t<T>, input_iterator_tag>;

template <class T>
constexpr bool is_forward_iterator_v = is_subcategory_of_v<category_t<T>, forward_iterator_tag>;

template <class T>
constexpr bool is_bidirectional_iterator_v =
    is_subcategory_of_v<category_t<T>, bidirectional_iterator_tag>;

template <class T>
constexpr bool is_random_access_iterator_v =
    is_subcategory_of_v<category_t<T>, random_access_iterator_tag>;

}  // namespace ac
