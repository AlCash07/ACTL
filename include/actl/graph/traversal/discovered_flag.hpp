/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/default_map.hpp>
#include <actl/graph/events.hpp>
#include <type_traits>

namespace ac {

template <class Map>
struct discovered_flag {
    static_assert(std::is_same_v<typename map_traits<Map>::value_type, bool>);

    using vertex = typename map_traits<Map>::key_type;

    void operator()(on_vertex_initialize, vertex u) { put(map, u, false); }
    void operator()(on_vertex_discover, vertex u) { put(map, u, true); }
    bool operator()(is_vertex_discovered, vertex u) { return get(map, u); }

    Map map;
};

template <class Map>
discovered_flag(Map&&) -> discovered_flag<Map>;

template <class Graph>
inline auto default_discovered_flag(const Graph& graph) {
    return discovered_flag{default_vertex_map<bool>(graph)};
}

}  // namespace ac
