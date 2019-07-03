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

#include <actl/std/utility.hpp>
#include <actl/util/type_traits.hpp>
#include <functional>

namespace ac {

template <class Key, class Ref, class Value = use_default, bool Readable = true,
          bool Writable = false, bool Invertible = false, bool Iterable = false, class Range = void>
struct map_traits_base {
    using key_type = Key;
    using reference = Ref;
    using value_type = deduce_t<Value, remove_cvref_t<Ref>>;
    using range_type = Range;

    static constexpr bool readable = Readable;
    static constexpr bool writable = Writable;
    static constexpr bool invertible = Invertible;
    static constexpr bool iterable = Iterable;
};

template <class Map, class Traits>
struct member_map_ops : Traits {
    using K = typename Traits::key_type;
    using V = typename Traits::value_type;

    static constexpr typename Traits::reference get(Map& map, K key) { return map.get(key); }
    static constexpr void put(Map& map, K key, V value) { map.put(key, value); }
    static constexpr K invert(Map& map, V value) { return map.invert(value); }
    static constexpr typename Traits::range_type map_range(Map& map) { return map.map_range(); }
};

template <class Map, class Traits>
struct default_map_put : Traits {
    static constexpr void put(Map& map, typename Traits::key_type key, typename Traits::value_type value) {
        get(map, key) = value;
    }
};

template <class T, class = void>
struct const_map_traits {};

template <class T>
struct const_map_traits<const T> : const_map_traits<T> {};

template <class T, class = void>
struct map_traits : const_map_traits<T> {};

template <class T>
struct map_traits<T&> : map_traits<T> {};

template <class T>
struct map_traits<std::reference_wrapper<T>> : map_traits<T> {};

template <class T>
using map_key_t = typename map_traits<T>::key_type;

template <class T>
using map_reference_t = typename map_traits<T>::reference;

template <class T>
using map_value_t = typename map_traits<T>::value_type;

template <class T>
using map_pair_t = std::pair<map_key_t<T>, map_reference_t<T>>;

template <class T>
using map_range_t = typename map_traits<T>::range_type;

template <class T>
using map_iterator_t = iterator_t<map_range_t<T>>;

template <class T, enable_int_if<map_traits<T>::readable> = 0>
inline map_reference_t<T> get(T&& map, map_key_t<T> key) {
    return map_traits<T>::get(map, key);
}

template <class T, enable_int_if<map_traits<T>::writable> = 0>
inline void put(T&& map, map_key_t<T> key, map_value_t<T> value) {
    return map_traits<T>::put(map, key, value);
}

template <class T, enable_int_if<map_traits<T>::invertible> = 0>
inline map_key_t<T> invert(T&& map, map_value_t<T> value) {
    return map_traits<T>::invert(map, value);
}

template <class T, enable_int_if<map_traits<T>::iterable> = 0>
inline map_range_t<T> map_range(T&& map) {
    return map_traits<T>::map_range(map);
}

}  // namespace ac
