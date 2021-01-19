// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/utility/nesting_depth.hpp>
#include <actl/meta/dependent.hpp>

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
struct range_tag               { using base = scalar_tag; };
struct output_range_tag        { using base = range_tag; };
struct input_range_tag         { using base = range_tag; };
struct forward_range_tag       { using base = input_range_tag; };
struct bidirectional_range_tag { using base = forward_range_tag; };
struct random_access_range_tag { using base = bidirectional_range_tag; };
struct contiguous_range_tag    { using base = random_access_range_tag; };
// clang-format on

namespace detail {

template <class C>
struct range_category;

template <>
struct range_category<std::output_iterator_tag> {
    using type = output_range_tag;
};

template <>
struct range_category<std::input_iterator_tag> {
    using type = input_range_tag;
};

template <>
struct range_category<std::forward_iterator_tag> {
    using type = forward_range_tag;
};

template <>
struct range_category<std::bidirectional_iterator_tag> {
    using type = bidirectional_range_tag;
};

template <>
struct range_category<std::random_access_iterator_tag> {
    using type = random_access_range_tag;
};

}  // namespace detail

template <class T>
struct category_sfinae<T, std::enable_if_t<is_range_v<T> && !is_contiguous_range_v<T>>>
    : detail::range_category<typename std::iterator_traits<iterator_t<T>>::iterator_category> {};

template <class T>
struct category_sfinae<T, std::enable_if_t<is_contiguous_range_v<T>>> {
    using type = contiguous_range_tag;
};

template <class T>
constexpr bool is_input_range_v = is_subcategory_of_v<category_t<T>, input_range_tag>;

template <class T>
constexpr bool is_output_range_v = is_subcategory_of_v<category_t<T>, output_range_tag>;

template <class T>
constexpr bool is_forward_range_v = is_subcategory_of_v<category_t<T>, forward_range_tag>;

template <class T>
constexpr bool is_bidirectional_range_v =
    is_subcategory_of_v<category_t<T>, bidirectional_range_tag>;

template <class T>
constexpr bool is_random_access_range_v =
    is_subcategory_of_v<category_t<T>, random_access_range_tag>;

template <class T>
struct nesting_depth<T, std::enable_if_t<is_range_v<T>>>
    : index_constant<1 + nesting_depth_v<value_type_t<T>>> {};

}  // namespace ac
