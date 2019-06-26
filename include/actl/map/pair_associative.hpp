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
    std::enable_if_t<is_unique_associative_container_v<C> && is_pair_associative_container_v<C>,
                     int>;

}

template <class AC>
struct map_types<AC, std::void_t<detail::enable_if_upac_t<AC>>> {
    using key_type = const typename AC::key_type;
    using reference = add_const_if_t<std::is_const_v<AC>, typename AC::mapped_type>;
};

// If key isn't present in container, default value is returned.
template <class AC, class Ref = typename map_traits<AC>::reference,
          detail::enable_if_upac_t<AC> = 0>
inline Ref get(AC& map, typename map_traits<AC>::key_type key) {
    auto it = map.find(key);
    return it == map.end() ? Ref{} : it->second;
}

template <class AC, detail::enable_if_upac_t<AC> = 0,
          std::enable_if_t<!std::is_const_v<AC>, int> = 0>
inline void put(AC& map, typename map_traits<AC>::key_type key,
                typename map_traits<AC>::value_type value) {
    map[key] = value;
}

template <class AC, detail::enable_if_upac_t<AC> = 0>
inline AC& map_range(AC& map) {
    return map;
}

}  // namespace ac
