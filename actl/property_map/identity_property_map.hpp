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
class identity_property_map : public creative_property_map<true, Key, Value> {
public:
    constexpr Value operator [] (const Key& key) const { return static_cast<Value>(key); }

    constexpr Key invert(const Value& value) const { return static_cast<Key>(value); }
};

/**
 * Property map that casts the reference. Preferable when the types are heavy or for wrapping.
 */
template <class Key, class Value = Key>
class identity_ref_property_map : public property_map<false, true, Key, Value> {
public:
    constexpr Value& operator [] (Key& key) const { return static_cast<Value&>(key); }

    constexpr const Value& operator [] (const Key& key) const {
        return static_cast<const Value&>(key);
    }

    constexpr const Key& invert(const Value& value) const { return static_cast<const Key&>(value); }
};

}  // namespace ac
