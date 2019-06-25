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

template <class T>
struct map_types {
    using key_type = typename T::key_type;
    using reference = typename T::reference;
};

template <class T>
struct map_types<T&> : map_types<T> {};

namespace detail {

template <class T, class K, class = void>
struct has_get : std::false_type {};

template <class T, class K>
struct has_get<T, K, std::void_t<decltype(get(std::declval<T&>(), std::declval<K>()))>>
    : std::true_type {};

template <class T, class K, class V, class = void>
struct has_put : std::false_type {};

template <class T, class K, class V>
struct has_put<T, K, V,
               std::void_t<decltype(put(std::declval<T&>(), std::declval<K>(), std::declval<V>()))>>
    : std::true_type {};

template <class T, class V, class = void>
struct has_invert : std::false_type {};

template <class T, class V>
struct has_invert<T, V, std::void_t<decltype(invert(std::declval<T&>(), std::declval<V>()))>>
    : std::true_type {};

template <class T, class = void>
struct has_range : std::false_type {};

template <class T>
struct has_range<T, std::void_t<decltype(map_range(std::declval<T&>()))>> : std::true_type {};

}  // namespace detail

template <class T>
struct map_traits : map_types<T> {
    using typename map_types<T>::key_type;
    using typename map_types<T>::reference;
    using value_type = remove_cvref_t<reference>;

    static constexpr bool readable = detail::has_get<T, key_type>::value;
    static constexpr bool writable = detail::has_put<T, key_type, value_type>::value;
    static constexpr bool invertible = detail::has_invert<T, value_type>::value;
    static constexpr bool iterable = detail::has_range<T>::value;
};

}  // namespace ac
