/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/vertex_initializer.hpp>

namespace ac {

template <class Map>
struct distance_recorder {
    void operator()(on_vertex_start, map_key_t<Map> u) { put(map, u, 0); }

    template <class E>
    void operator()(on_tree_edge_examine, E e) {
        put(map, e.target(), get(map, e.source()) + 1);
    }

    Map map;
};

template <class Map>
distance_recorder(Map&&) -> distance_recorder<Map>;

template <class Map, class T>
inline vertex_initializer<distance_recorder<Map>> make_distance_recorder(Map&& distance, T value) {
    return {{std::forward<Map>(distance)}, value};
}

template <class Graph>
inline auto make_default_distance_recorder(const Graph& graph) {
    return make_distance_recorder(make_default_vertex_map<int>(graph), -1);
}

}  // namespace ac
