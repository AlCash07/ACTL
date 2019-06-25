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
struct identity_map {
    using key_type = Key;
    using reference = Value;

    friend constexpr Value get(identity_map, Key key) { return static_cast<Value>(key); }

    template <bool B = std::is_convertible_v<Value, Key>, class = std::enable_if_t<B>>
    friend constexpr Key map_invert(identity_map, Value value) {
        return static_cast<Key>(value);
    }
};

}  // namespace ac
