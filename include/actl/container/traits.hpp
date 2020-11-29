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
#include <actl/range/traits.hpp>
#include <actl/util/rebind.hpp>
#include <actl/util/type_traits.hpp>

namespace ac {

// Virtual inheritance is used because of inheritance diamonds.
struct container_tag {};

struct associative_container_tag : virtual container_tag {};

struct simple_associative_container_tag : virtual associative_container_tag {};
struct pair_associative_container_tag : virtual associative_container_tag {};

template <class C>
struct container_category {};

template <class C>
struct container_category<const C> : container_category<C> {};

template <class C, class Tag>
inline constexpr bool has_container_tag_v = std::is_base_of_v<Tag, container_category<C>>;

template <class C>
inline constexpr bool is_associative_container_v =
    has_container_tag_v<C, associative_container_tag>;

template <class C>
inline constexpr bool is_simple_associative_container_v =
    has_container_tag_v<C, simple_associative_container_tag>;

template <class C>
inline constexpr bool is_pair_associative_container_v =
    has_container_tag_v<C, pair_associative_container_tag>;

template <class C>
inline constexpr bool is_sequence_container_v = is_container_v<C> && !is_associative_container_v<C>;

template <class C, class To>
using rebind_container_t = std::conditional_t<std::is_same_v<C, none> || std::is_same_v<To, none>,
                                              dummy_container, rebind_t<C, To>>;

/* std::data extension for smart pointers */

template <class T, class = void>
struct is_smart_pointer : std::false_type {};

template <class T>
struct is_smart_pointer<T, std::void_t<decltype(std::declval<T>().get())>> :
    std::is_pointer<decltype(std::declval<T>().get())> {};

template <class T>
inline auto data(T&& x) {
    if constexpr (is_smart_pointer<T>::value) {
        return x.get();
    } else {
        return std::data(x);
    }
}

}  // namespace ac
