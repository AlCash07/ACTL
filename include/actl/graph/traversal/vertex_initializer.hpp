/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/events.hpp>
#include <actl/map/traits.hpp>

namespace ac {

template <class Base>
struct vertex_initializer : Base {
    using Base::map;
    using map_t = decltype(map);
    using vertex = map_key_t<map_t>;

    using Base::operator();

    void operator()(on_vertex_initialize, vertex u) { put(map, u, value); }
    bool operator()(is_vertex_discovered, vertex u) { return get(map, u) != value; }

    map_value_t<map_t> value;
};

}  // namespace ac
