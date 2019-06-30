/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/traits.hpp>

namespace ac {

template <class Key, class Value = void>
class dummy_map {
    static constexpr bool RW = !std::is_same_v<Value, void>;

public:
    using traits = map_traits_base<Key, Value, Value, RW, RW>;

    constexpr static Value get(Key) { return Value{}; }

    template <class V = Value>
    constexpr static void put(Key, V) {}
};

template <class K, class V>
struct const_map_traits<dummy_map<K, V>> : dummy_map<K, V>::traits {};

}  // namespace ac
