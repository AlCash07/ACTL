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
#include <utility>

namespace ac {

template <class Key, class Ref, bool Readable = true, bool Writable = false,
          bool Invertible = false, bool Iterable = false>
struct map_traits_base {
    using key_type = Key;
    using reference = Ref;
    using value_type = remove_cvref_t<Ref>;
    using pair_type = std::pair<Key, Ref>;

    static constexpr bool readable = Readable;
    static constexpr bool writable = Writable;
    static constexpr bool invertible = Invertible;
    static constexpr bool iterable = Iterable;
};

template <class T, class = void>
struct const_map_traits {};

template <class T>
struct const_map_traits<const T, void> : const_map_traits<T> {};

template <class T, class = void>
struct map_traits : const_map_traits<T> {};

template <class T>
struct map_traits<T&, void> : map_traits<T> {};

template <class T>
using map_key_t = typename map_traits<T>::key_type;

template <class T>
using map_reference_t = typename map_traits<T>::reference;

template <class T>
using map_value_t = typename map_traits<T>::value_type;

template <class T>
using map_pair_t = typename map_traits<T>::pair_type;

}  // namespace ac
