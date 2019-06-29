/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/map.hpp>

namespace ac {

/**
 * Property map that simply casts the key. Fitting to be a default map.
 */
template <class Key, class Value = Key>
class identity_map {
public:
    friend constexpr Value get(identity_map, Key key) { return static_cast<Value>(key); }

    template <bool B = map_traits<identity_map>::invertible, std::enable_if_t<B, int> = 0>
    friend constexpr Key invert(identity_map, Value value) {
        return static_cast<Key>(value);
    }
};

template <class K, class V>
struct const_map_traits<identity_map<K, V>>
    : map_traits_base<K, V, true, false, std::is_convertible_v<V, K>> {};

}  // namespace ac
