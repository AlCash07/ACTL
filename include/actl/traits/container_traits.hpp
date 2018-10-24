/***************************************************************************************************
 * Standard containers traits and categorization.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/dummy_container.hpp>
#include <actl/traits/rebind.hpp>

namespace ac {

template <class C>
struct value_type {
    using type = typename C::value_type;
};

template <class T, size_t N>
struct value_type<T[N]> {
    using type = T;
};

template <>
struct value_type<none> {
    using type = none;
};

template <class C>
using value_type_t = typename value_type<C>::type;

// Virtual inheritance is used because of inheritance diamonds.
struct container_tag {};

struct sequence_container_tag : virtual container_tag {};

struct random_access_container_tag : virtual sequence_container_tag {};

struct associative_container_tag : virtual container_tag {};

struct sorted_associative_container_tag : virtual associative_container_tag {};

struct unique_associative_container_tag : virtual associative_container_tag {};
struct multiple_associative_container_tag : virtual associative_container_tag {};

struct simple_associative_container_tag : virtual associative_container_tag {};
struct pair_associative_container_tag : virtual associative_container_tag {};

template <class C>
struct container_category : none {};

template <class T, size_t N>
struct container_category<T[N]> : random_access_container_tag {};

template <>
struct container_category<dummy_container> : random_access_container_tag {};

template <class C>
inline constexpr bool is_container_v = !std::is_base_of_v<none, container_category<C>>;

template <class C, class Tag>
inline constexpr bool has_container_tag_v = std::is_base_of_v<Tag, container_category<C>>;

template <class C>
inline constexpr bool is_sequence_container_v = has_container_tag_v<C, sequence_container_tag>;

template <class C>
inline constexpr bool is_random_access_container_v =
    has_container_tag_v<C, random_access_container_tag>;

template <class C>
inline constexpr bool is_associative_container_v =
    has_container_tag_v<C, associative_container_tag>;

template <class C>
inline constexpr bool is_sorted_associative_container_v =
    has_container_tag_v<C, sorted_associative_container_tag>;

template <class C>
inline constexpr bool is_unique_associative_container_v =
    has_container_tag_v<C, unique_associative_container_tag>;

template <class C>
inline constexpr bool is_multiple_associative_container_v =
    has_container_tag_v<C, multiple_associative_container_tag>;

template <class C>
inline constexpr bool is_simple_associative_container_v =
    has_container_tag_v<C, simple_associative_container_tag>;

template <class C>
inline constexpr bool is_pair_associative_container_v =
    has_container_tag_v<C, pair_associative_container_tag>;

template <class C, class To>
using rebind_container_t = std::conditional_t<std::is_same_v<C, none> || std::is_same_v<To, none>,
                                              dummy_container, rebind_t<C, To>>;

}  // namespace ac
