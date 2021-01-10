// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/traits.hpp>
#include <actl/traits/dependent.hpp>

namespace ac {

template <class T, class = void>
struct is_range : std::false_type {};

template <class T>  // & is needed for C arrays
struct is_range<T,
                std::void_t<decltype(std::begin(std::declval<T&>()), std::end(std::declval<T&>()))>>
    : std::true_type {};

template <class T>
constexpr bool is_range_v = is_range<T>::value;

template <class T, class = void>
struct is_contiguous_range : std::false_type {};

template <class T>  // & is needed for C arrays
struct is_contiguous_range<
    T, std::void_t<decltype(std::data(std::declval<T&>()), std::size(std::declval<T&>()))>>
    : std::true_type {};

template <class T>
constexpr bool is_contiguous_range_v = is_contiguous_range<T>::value;

// clang-format off
template <class T> struct range_tag               { struct has_nested; using base = scalar_tag; };
template <class T> struct output_range_tag        { struct has_nested; using base = range_tag<T>; };
template <class T> struct input_range_tag         { struct has_nested; using base = range_tag<T>; };
template <class T> struct forward_range_tag       { struct has_nested; using base = input_range_tag<T>; };
template <class T> struct bidirectional_range_tag { struct has_nested; using base = forward_range_tag<T>; };
template <class T> struct random_access_range_tag { struct has_nested; using base = bidirectional_range_tag<T>; };
template <class T> struct contiguous_range_tag    { struct has_nested; using base = random_access_range_tag<T>; };
// clang-format on

namespace detail {

template <class C, class V>
struct range_category;

template <class V>
struct range_category<std::output_iterator_tag, V> {
    using type = output_range_tag<V>;
};

template <class V>
struct range_category<std::input_iterator_tag, V> {
    using type = input_range_tag<V>;
};

template <class V>
struct range_category<std::forward_iterator_tag, V> {
    using type = forward_range_tag<V>;
};

template <class V>
struct range_category<std::bidirectional_iterator_tag, V> {
    using type = bidirectional_range_tag<V>;
};

template <class V>
struct range_category<std::random_access_iterator_tag, V> {
    using type = random_access_range_tag<V>;
};

}  // namespace detail

template <class T>
struct category_sfinae<T, std::enable_if_t<is_range_v<T> && !is_contiguous_range_v<T>>>
    : detail::range_category<typename std::iterator_traits<iterator_t<T>>::iterator_category,
                             category_t<value_type_t<T>>> {};

template <class T>
struct category_sfinae<T, std::enable_if_t<is_contiguous_range_v<T>>> {
    using type = contiguous_range_tag<category_t<value_type_t<T>>>;
};

template <class T>
constexpr bool is_input_range_v = is_subcategory_of_template_v<category_t<T>, input_range_tag>;

template <class T>
constexpr bool is_output_range_v = is_subcategory_of_template_v<category_t<T>, output_range_tag>;

template <class T>
constexpr bool is_forward_range_v = is_subcategory_of_template_v<category_t<T>, forward_range_tag>;

template <class T>
constexpr bool is_bidirectional_range_v =
    is_subcategory_of_template_v<category_t<T>, bidirectional_range_tag>;

template <class T>
constexpr bool is_random_access_range_v =
    is_subcategory_of_template_v<category_t<T>, random_access_range_tag>;

}  // namespace ac
