// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/traits/category.hpp>
#include <actl/meta/nesting_depth.hpp>
#include <actl/range/access/all.hpp>
#include <actl/range/traits/dependent.hpp>

namespace ac {

// clang-format off
struct range_tag {};
struct input_range_tag         : range_tag {};
struct forward_range_tag       : input_range_tag {};
struct bidirectional_range_tag : forward_range_tag {};
struct random_access_range_tag : bidirectional_range_tag {};
struct contiguous_range_tag    : random_access_range_tag {};
// clang-format on

namespace detail {

template <class C>
struct iter_to_range_category;

template <>
struct iter_to_range_category<std::input_iterator_tag>
{
    using type = input_range_tag;
};

template <>
struct iter_to_range_category<std::forward_iterator_tag>
{
    using type = forward_range_tag;
};

template <>
struct iter_to_range_category<std::bidirectional_iterator_tag>
{
    using type = bidirectional_range_tag;
};

template <>
struct iter_to_range_category<std::random_access_iterator_tag>
{
    using type = random_access_range_tag;
};

template <class T, class = void>
struct range_category_impl
    : iter_to_range_category<iter_category_t<range_iterator_t<T>>>
{};

template <class T>
struct range_category_impl<
    T,
    std::void_t<decltype( // & is needed for C arrays
        ranges::data(std::declval<T&>()),
        ranges::size(std::declval<T&>()))>>
{
    using type = contiguous_range_tag;
};

} // namespace detail

template <class T, class = void>
struct range_category
{};

template <class T>
struct range_category<
    T,
    std::void_t<decltype( // & is needed for C arrays
        ranges::begin(std::declval<T&>()),
        ranges::end(std::declval<T&>()))>> : detail::range_category_impl<T>
{};

template <class T>
using range_category_t = typename range_category<T>::type;

namespace detail {

template <class T, class Category, class = void>
struct has_range_category : std::false_type
{};

template <class T, class Category>
struct has_range_category<T, Category, std::void_t<range_category_t<T>>>
    : std::is_base_of<Category, range_category_t<T>>
{};

} // namespace detail

template <class T>
inline constexpr bool is_range_v =
    detail::has_range_category<T, range_tag>::value;

template <class T>
inline constexpr bool is_input_range_v =
    detail::has_range_category<T, input_range_tag>::value;

template <class T>
inline constexpr bool is_forward_range_v =
    detail::has_range_category<T, forward_range_tag>::value;

template <class T>
inline constexpr bool is_bidirectional_range_v =
    detail::has_range_category<T, bidirectional_range_tag>::value;

template <class T>
inline constexpr bool is_random_access_range_v =
    detail::has_range_category<T, random_access_range_tag>::value;

template <class T>
inline constexpr bool is_contiguous_range_v =
    detail::has_range_category<T, contiguous_range_tag>::value;

template <class T>
struct nesting_depth<T, std::enable_if_t<is_range_v<T>>>
    : size_constant<1 + nesting_depth_v<range_value_t<T>>>
{};

} // namespace ac
