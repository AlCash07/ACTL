/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/events.hpp>
#include <actl/map/property_map.hpp>

namespace ac {

template <class Base>
struct vertex_initializer : Base {
    using Base::map;
    using map_t = decltype(map);
    using vertex = typename map_traits<map_t>::key_type;

    using Base::operator();

    void operator()(on_vertex_initialize, vertex u) { put(map, u, value); }
    bool operator()(is_vertex_discovered, vertex u) { return get(map, u) != value; }

    typename map_traits<map_t>::value_type value;
};

}  // namespace ac
