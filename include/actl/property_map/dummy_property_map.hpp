/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>

namespace ac {

// Value is void by default to produce compilation error where get result is used.
template <class Key, class Value = void>
class dummy_property_map
    : public property_map<Key, Value, Value, false, false, std::is_same_v<Value, void>> {
    friend Value get(dummy_property_map, Key) { return Value(); }
};

template <class K, class V>
inline void put(dummy_property_map<K, V>, K, V) {}

}  // namespace ac
