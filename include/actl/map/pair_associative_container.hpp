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

template <class T>
using enable_int_if_upac =
    std::enable_if_t<is_unique_associative_container_v<T> && is_pair_associative_container_v<T>,
                     int>;

template <class AC, bool C>
using upac_traits =
    map_traits_base<const typename AC::key_type, typename AC::mapped_type, true, !C, false, true>;

}  // namespace detail

template <class AC>
struct map_traits<AC, std::void_t<detail::enable_int_if_upac<AC>>>
    : detail::upac_traits<AC, false> {};

template <class AC>
struct map_traits<const AC, std::void_t<detail::enable_int_if_upac<AC>>>
    : detail::upac_traits<AC, true> {};

// If key isn't present in container, default value is returned.
template <class AC, detail::enable_int_if_upac<AC> = 0>
inline map_reference_t<AC> get(const AC& map, map_key_t<AC> key) {
    auto it = map.find(key);
    return it == map.end() ? map_reference_t<AC>{} : it->second;
}

template <class AC, detail::enable_int_if_upac<AC> = 0>
inline void put(AC& map, map_key_t<AC> key, map_value_t<AC> value) {
    map[key] = value;
}

template <class AC, detail::enable_int_if_upac<std::remove_const_t<AC>> = 0>
inline AC& map_range(AC& map) {
    return map;
}

}  // namespace ac
