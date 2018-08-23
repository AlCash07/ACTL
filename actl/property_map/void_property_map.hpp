/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>

namespace ac {

template <class Key, class Value = void>
class void_property_map : public property_map<Key, Value, Value, false> {
    friend Value get(void_property_map, Key) { return Value{}; }

    friend void put(void_property_map, Key, Value) {}
};

}  // namespace ac
