/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/map.hpp>
#include <actl/traits/container_traits.hpp>

namespace ac {

namespace detail {

template <class T, class C = std::remove_const_t<T>>
using enable_if_upac_t =
    std::enable_if_t<is_unique_associative_container_v<C> && is_pair_associative_container_v<C>>;

}  // namespace detail

template <class AC>
struct map_traits<AC, detail::enable_if_upac_t<AC>>
    : map_traits_base<const typename AC::key_type, typename AC::mapped_type, true,
                      !std::is_const_v<AC>, false, true, AC&> {};

template <class AC>
struct map_ops<AC, detail::enable_if_upac_t<AC>> {
    // If key isn't present in container, default value is returned.
    static map_reference_t<AC> get(const AC& map, map_key_t<AC> key) {
        auto it = map.find(key);
        return it == map.end() ? map_reference_t<AC>{} : it->second;
    }

    static void put(AC& map, map_key_t<AC> key, map_value_t<AC> value) { map[key] = value; }

    static AC& map_range(AC& map) { return map; }
};

}  // namespace ac
