/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/property_map.hpp>

namespace ac {

/**
 * Property map that simply casts the key. Fitting to be a default map.
 */
template <class Key, class Value = Key>
class identity_map {
public:
    using key_type = Key;
    using reference = Value;

    friend constexpr Value get(identity_map, Key key) { return static_cast<Value>(key); }
};

template <class K, class V, class = std::enable_if_t<std::is_convertible_v<V, K>>>
inline constexpr K invert(identity_map<K, V>, V value) {
    return static_cast<K>(value);
}

}  // namespace ac
