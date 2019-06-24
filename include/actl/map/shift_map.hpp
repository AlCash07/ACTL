/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/property_map.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

/**
 * Property map that shifts key domain by the given offset (with casting).
 */
template <class Key, class Value = Key>
class shift_map : public property_map<Key, Value, Value, true> {
public:
    explicit constexpr shift_map(Key offset) : offset_{offset} {}

    friend constexpr Value get(const shift_map& pm, Key key) {
        return static_cast<Value>(key - pm.offset_);
    }

    constexpr Key invert(Value value) const { return static_cast<Key>(value) + offset_; }

private:
    const Key offset_;
};

template <class Value = use_default, class Key>
inline auto make_shift_map(Key offset) {
    return shift_map<Key, deduce_type_t<Value, Key>>(offset);
}

/**
 * Shift property map with offset known at compile-time.
 */
template <auto Offset, class Value = decltype(Offset)>
class static_shift_map : public property_map<decltype(Offset), Value, Value, true> {
    using Key = decltype(Offset);

public:
    friend constexpr Value get(static_shift_map, Key key) {
        return static_cast<Value>(key - Offset);
    }

    constexpr Key invert(Value value) const { return static_cast<Key>(value) + Offset; }
};

}  // namespace ac
