/***************************************************************************************************
 * Standard containers traits and categorization.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

template <class C>
struct value_type {
    using type = typename C::value_type;
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
struct container_traits;

template <class T, size_t N>
struct container_traits<T[N]> {
    using category = random_access_container_tag;

    template <class U>
    using rebind = U[N];
};

template <class A, class T>
using rebind_allocator_t = typename A::template rebind<T>::other;

template <class C, class... Ts>
using rebind_container_t = typename container_traits<C>::template rebind<Ts...>;

namespace detail {

template <class C, class = void>
struct is_container : std::false_type {};

template <class C>
struct is_container<C, std::void_t<typename container_traits<C>::category>> : std::true_type {};

template <class C, class Tag, bool = true>
struct has_container_tag : std::is_base_of<Tag, typename container_traits<C>::category> {};

template <class C, class Tag>
struct has_container_tag<C, Tag, false> : std::false_type {};

}  // namespace detail

template <class C>
inline constexpr bool is_container_v = detail::is_container<C>::value;

template <class C, class Tag>
inline constexpr bool has_container_tag_v =
    detail::has_container_tag<C, Tag, is_container_v<C>>::value;

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

}  // namespace ac
