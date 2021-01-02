// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/map/traits.hpp>
#include <actl/range/traits/range_traits.hpp>

namespace ac {

namespace detail {

template <class T>
using enable_if_upac_t = std::enable_if_t<is_unique_range_v<T> && is_pair_associative_range_v<T>>;

}  // namespace detail

template <class AC>
struct map_traits<AC, detail::enable_if_upac_t<AC>>
    : map_traits_base<const typename AC::key_type, typename AC::mapped_type, use_default, true,
                      !std::is_const_v<AC>, false, true, AC&> {};

template <class AC>
struct map_ops<AC, detail::enable_if_upac_t<AC>> {
    // If key isn't present in container, default value is returned.
    static map_reference_t<AC> get(const AC& map, map_key_t<AC> key) {
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

}  // namespace ac
