/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/default_property_map.hpp>
#include <actl/graph/traversal/vertex_initializer.hpp>

namespace ac {

template <class Map>
struct predecessor_edge_recorder : property_map_wrapper_t<Map> {
    template <class E>
    void operator()(on_tree_edge, E e) {
        put(*this, e.target(), e);
    }

    template <class E>
    void operator()(on_edge_relaxed, E e) {
        operator()(on_tree_edge{}, e);
    }
};

template <class Map>
inline predecessor_edge_recorder<Map> make_predecessor_edge_recorder(Map&& predecessor_edge) {
    return {std::forward<Map>(predecessor_edge)};
}

template <class Graph>
inline auto make_default_predecessor_edge_recorder(const Graph& graph) {
    return make_predecessor_edge_recorder(
        make_default_vertex_property_map<typename Graph::edge>(graph));
}

}  // namespace ac
