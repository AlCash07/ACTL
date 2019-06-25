/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/property_map.hpp>

namespace ac {

// Value is void by default to produce compilation error where get result is used.
template <class Key, class Value = void>
class dummy_map {
public:
    using key_type = Key;
    using reference = Value;

    friend constexpr Value get(dummy_map, Key) { return Value(); }
};

template <class K, class V, class = std::enable_if_t<!std::is_same_v<V, void>>>
inline constexpr void put(dummy_map<K, V>, K, V) {}

}  // namespace ac
