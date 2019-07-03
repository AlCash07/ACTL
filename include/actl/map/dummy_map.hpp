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
class dummy_map {};

template <class K, class V>
struct const_map_traits<dummy_map<K, V>> {
    using DM = dummy_map<K, V>;

    using key_type = K;
    using reference = V;
    using value_type = V;

    static constexpr bool readable = !std::is_same_v<V, void>;
    static constexpr bool writable = readable;
    static constexpr bool invertible = false;
    static constexpr bool iterable = false;

    static constexpr V get(DM, K) { return {}; }

    static constexpr void put(DM, K, V) {}
};

}  // namespace ac
