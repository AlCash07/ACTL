// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/scalar.hpp>
#include <actl/category/utility/is_subcategory_of_template.hpp>
#include <iterator>

namespace ac {

namespace detail {

template <class T, class = void>
struct is_iter : std::false_type {};

template <class T>
struct is_iter<T, void_t<typename std::iterator_traits<T>::iterator_category>> : std::true_type {};

}  // namespace detail

template <class T, class = void>
struct is_iterator : detail::is_iter<T> {};

template <class T>
struct is_iterator<T*, std::enable_if_t<!std::is_object_v<T>>> : std::false_type {};

template <class T>
constexpr bool is_iterator_v = is_iterator<T>::value;

// clang-format off
template <class T> struct iterator_tag               { using base = scalar_tag; };
template <class T> struct output_iterator_tag        { using base = iterator_tag<T>; };
template <class T> struct input_iterator_tag         { using base = iterator_tag<T>; };
template <class T> struct forward_iterator_tag       { using base = input_iterator_tag<T>; };
template <class T> struct bidirectional_iterator_tag { using base = forward_iterator_tag<T>; };
template <class T> struct random_access_iterator_tag { using base = bidirectional_iterator_tag<T>; };
// clang-format on

namespace detail {

template <class C, class V>
struct iter_category;

template <class V>
struct iter_category<std::output_iterator_tag, V> {
    using type = output_iterator_tag<V>;
};

template <class V>
struct iter_category<std::input_iterator_tag, V> {
    using type = input_iterator_tag<V>;
};

template <class V>
struct iter_category<std::forward_iterator_tag, V> {
    using type = forward_iterator_tag<V>;
};

template <class V>
struct iter_category<std::bidirectional_iterator_tag, V> {
    using type = bidirectional_iterator_tag<V>;
};

template <class V>
struct iter_category<std::random_access_iterator_tag, V> {
    using type = random_access_iterator_tag<V>;
};

}  // namespace detail

template <class T>
struct category_sfinae<T, std::enable_if_t<is_iterator_v<T>>>
    : detail::iter_category<typename std::iterator_traits<T>::iterator_category,
                            category_t<typename std::iterator_traits<T>::value_type>> {};

template <class T>
constexpr bool is_input_iterator_v =
    is_subcategory_of_template_v<category_t<T>, input_iterator_tag>;

template <class T>
constexpr bool is_output_iterator_v =
    is_subcategory_of_template_v<category_t<T>, output_iterator_tag>;

template <class T>
constexpr bool is_forward_iterator_v =
    is_subcategory_of_template_v<category_t<T>, forward_iterator_tag>;

template <class T>
constexpr bool is_bidirectional_iterator_v =
    is_subcategory_of_template_v<category_t<T>, bidirectional_iterator_tag>;

template <class T>
constexpr bool is_random_access_iterator_v =
    is_subcategory_of_template_v<category_t<T>, random_access_iterator_tag>;

}  // namespace ac
