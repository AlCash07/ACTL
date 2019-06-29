/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/map.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

/**
 * Property map that shifts key domain by the given offset (with casting).
 */
template <class Key, class Value = Key>
class shift_map {
public:
    explicit constexpr shift_map(Key offset) : offset_{offset} {}

    friend constexpr Value get(const shift_map& map, Key key) {
        return static_cast<Value>(key - map.offset_);
    }

    friend constexpr Key invert(const shift_map& map, Value value) {
        return static_cast<Key>(value) + map.offset_;
    }

private:
    const Key offset_;
};

template <class Value = use_default, class Key>
inline auto make_shift_map(Key offset) {
    return shift_map<Key, deduce_type_t<Value, Key>>(offset);
}

template <class K, class V>
struct const_map_traits<shift_map<K, V>> : map_traits_base<K, V, true, false, true> {};

/**
 * Shift property map with offset known at compile-time.
 */
template <auto Offset, class Value = decltype(Offset)>
class static_shift_map {
    using Key = decltype(Offset);

public:
    friend constexpr Value get(static_shift_map, Key key) {
        return static_cast<Value>(key - Offset);
    }

    friend constexpr Key invert(static_shift_map, Value value) {
        return static_cast<Key>(value) + Offset;
    }
};

template <auto O, class V>
struct const_map_traits<static_shift_map<O, V>>
    : map_traits_base<decltype(O), V, true, false, true> {};

}  // namespace ac
