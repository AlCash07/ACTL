/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>

namespace ac {

/**
 * Property map that simply casts the key. Fitting to be a default map.
 */
template <class Key, class Value = Key>
class identity_property_map
    : public property_map<Key, Value, Value, is_static_castable<Value, Key>::value> {
public:
    friend Value get(identity_property_map&, Key key) { return static_cast<Value>(key); }

    constexpr Key invert(Value value) { return static_cast<Key>(value); }
};

}  // namespace ac
