// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>
#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>

namespace ac {

template<class AC>
    requires UniqueRange<AC> && PairAssociativeRange<AC>
struct map_traits<AC>
    : map_traits_base<
          typename AC::key_type,
          typename AC::mapped_type,
          use_default,
          true,
          !std::is_const_v<AC>,
          false,
          true,
          AC&> {};

template<class AC>
    requires UniqueRange<AC> && PairAssociativeRange<AC>
struct map_ops<AC> {
    // If key isn't present in container, default value is returned.
    static map_reference_t<AC> get(AC const& map, map_key_t<AC> key) {
        auto it = map.find(key);
        return it == map.end() ? map_reference_t<AC>{} : it->second;
    }

    static void put(AC& map, map_key_t<AC> key, map_value_t<AC> value) {
        map[key] = value;
    }

    static AC& map_range(AC& map) {
        return map;
    }
};

} // namespace ac
