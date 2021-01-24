// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>

namespace ac {

/// Property map that simply casts the key. Fitting to be a default map.
template <class Key, class Value = Key>
class identity_map {
public:
    constexpr static Value get(Key key) {
        return static_cast<Value>(key);
    }

    constexpr static Key invert(Value value) {
        return static_cast<Key>(value);
    }
};

template <class K, class V>
struct const_map_traits<identity_map<K, V>>
    : map_traits_base<
          K,
          V,
          use_default,
          std::is_convertible_v<K, V>,
          false,
          std::is_convertible_v<V, K>> {};

}  // namespace ac
