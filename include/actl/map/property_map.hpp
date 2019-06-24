/***************************************************************************************************
 * Property maps inspired by boost, but much more powerful.
 * Besides access via get and put functions, property maps can support:
 * - inverse mapping (invert method);
 * - traversal of container maps (begin(), end() methods);
 * - clear method (not formalized yet).
 *
 * Custom property maps must inherit from property_map_base, which is done more conveniently by
 * inheriting from property_map, which defines all required nested types and static variables;
 * the last template parameters have suitable defaults for the case when mapped value is computed on
 * the fly.
 *
 * map_traits is a helper class that defines nested types and static variables for custom
 * property maps as well as for random access iterators (which can be used as property maps from
 * integer domain).
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/iterator_traits.hpp>
#include <type_traits>

namespace ac {

struct property_map_base {};

template <class Key, class Value, class Ref, bool Invertible, bool Iterable = false,
          bool Writable = false>
struct property_map : property_map_base {
    using key_type = Key;
    using value_type = Value;
    using reference = Ref;

    static constexpr bool invertible = Invertible;
    static constexpr bool iterable = Iterable;
    static constexpr bool writable = Writable;
};

// Inherit from this class to generate put(map, key, value) { get(map, key) = value; }.
template <class Map>
struct put_helper : property_map_base {};

template <class It>
class iterator_map : public put_helper<iterator_map<It>> {
    It it_;

public:
    static_assert(is_random_access_iterator_v<It>);

    using key_type = int;
    using value_type = typename std::iterator_traits<It>::value_type;
    using reference = typename std::iterator_traits<It>::reference;

    // TODO: if invert is guaranteed to take the result of get then invertible can be true.
    static constexpr bool invertible = false;
    static constexpr bool iterable = false;
    static constexpr bool writable = std::is_assignable_v<reference, value_type>;

    constexpr iterator_map(It it) : it_{it} {}

    friend reference get(const iterator_map& map, key_type key) { return map.it_[key]; }
};

namespace detail {

template <class Map, bool IsMap = true>
struct map_traits_impl {
    using key_type = typename Map::key_type;
    using value_type = typename Map::value_type;
    using reference = typename Map::reference;
    using wrapper = Map;

    static constexpr bool invertible = Map::invertible;
    static constexpr bool iterable = Map::iterable;
    static constexpr bool writable = Map::writable;
};

template <class It>
struct map_traits_impl<It, false> : map_traits_impl<iterator_map<It>> {};

}  // namespace detail

template <class Map>
struct map_traits : detail::map_traits_impl<Map, std::is_base_of_v<property_map_base, Map>> {};

template <class Map>
struct map_traits<Map&> : map_traits<Map> {};

// This type can be used as base class even when Map is a pointer.
template <class Map>
using map_wrapper_t = typename map_traits<Map>::wrapper;

// const It& disallows conversion from array to pointer.
template <class It, std::enable_if_t<is_random_access_iterator_v<It>, int> = 0>
inline typename map_traits<It>::reference get(const It& it, int key) {
    return it[key];
}

template <class It,
          std::enable_if_t<is_random_access_iterator_v<It> && map_traits<It>::writable, int> = 0>
inline void put(const It& it, int key, typename map_traits<It>::value_type value) {
    it[key] = value;
}

template <class Map, std::enable_if_t<map_traits<Map>::writable, int> = 0>
inline void put(const put_helper<Map>& map, typename map_traits<Map>::key_type key,
                typename map_traits<Map>::value_type value) {
    get(static_cast<const Map&>(map), key) = value;
}

}  // namespace ac
