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
struct predecessor_edge_recorder {
    template <class E>
    void operator()(on_tree_edge, E e) {
        put(map, e.target(), e);
    }

    template <class E>
    void operator()(on_edge_relaxed, E e) {
        operator()(on_tree_edge{}, e);
    }

    Map map;
};

template <class Map>
predecessor_edge_recorder(Map&&) -> predecessor_edge_recorder<Map>;

template <class Graph>
inline auto make_default_predecessor_edge_recorder(const Graph& graph) {
    return predecessor_edge_recorder{make_default_vertex_map<typename Graph::edge>(graph)};
}

}  // namespace ac
