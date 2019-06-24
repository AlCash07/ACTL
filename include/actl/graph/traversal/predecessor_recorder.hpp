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
struct predecessor_recorder {
    void operator()(on_vertex_start, typename map_traits<Map>::key_type u) { put(map, u, u); }

    template <class E>
    void operator()(on_tree_edge, E e) {
        put(map, e.target(), e.source());
    }

    template <class E>
    void operator()(on_edge_relaxed, E e) {
        operator()(on_tree_edge{}, e);
    }

    Map map;
};

template <class Map>
predecessor_recorder(Map&&) -> predecessor_recorder<Map>;

template <class Map, class T>
inline vertex_initializer<predecessor_recorder<Map>> make_predecessor_recorder(Map&& predecessor,
                                                                               T     value) {
    return {{std::forward<Map>(predecessor)}, value};
}

template <class Graph>
inline auto make_default_predecessor_recorder(const Graph& graph) {
    return make_predecessor_recorder(make_default_vertex_map<typename Graph::vertex>(graph),
                                     graph.null_vertex());
}

}  // namespace ac
