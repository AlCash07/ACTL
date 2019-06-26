/***************************************************************************************************
 * Maps inspired by boost property maps, but much more powerful. Supported functions:
 * - get(Map&, Key) : read;
 * - put(Map&, Key, Value) : write;
 * - invert(Map&, Value) : inverse mapping;
 * - map_range(Map&): traversal of key-value pairs.
 *
 * map_traits is a helper class that defines nested types and static variables.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/type_traits.hpp>

namespace ac {

template <class T, class = void>
struct map_types {
    using key_type = typename T::key_type;
    using reference = typename T::reference;
};

template <class T>
struct map_types<T&, void> : map_types<T> {};

namespace detail {

template <class, class... Ts>
struct has_get : std::false_type {};

template <class... Ts>
struct has_get<std::void_t<decltype(get(std::declval<Ts>()...))>, Ts...> : std::true_type {};

template <class, class... Ts>
struct has_put : std::false_type {};

template <class... Ts>
struct has_put<std::void_t<decltype(put(std::declval<Ts>()...))>, Ts...> : std::true_type {};

template <class, class... Ts>
struct has_invert : std::false_type {};

template <class... Ts>
struct has_invert<std::void_t<decltype(invert(std::declval<Ts>()...))>, Ts...> : std::true_type {};

template <class T, class = void>
struct has_range : std::false_type {};

template <class T>
struct has_range<T, std::void_t<decltype(map_range(std::declval<T>()))>> : std::true_type {};

}  // namespace detail

template <class T>
struct map_traits : map_types<T> {
    using typename map_types<T>::key_type;
    using typename map_types<T>::reference;
    using value_type = remove_cvref_t<reference>;

    static constexpr bool readable = detail::has_get<void, T&, key_type>::value;
    static constexpr bool writable = detail::has_put<void, T&, key_type, value_type>::value;
    static constexpr bool invertible = detail::has_invert<void, T&, value_type>::value;
    static constexpr bool iterable = detail::has_range<T&>::value;
};

}  // namespace ac
