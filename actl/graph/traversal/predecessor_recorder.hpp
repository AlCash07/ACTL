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
struct predecessor_recorder : property_map_wrapper_t<Map> {
    void operator()(on_vertex_start, typename property_traits<Map>::key_type u) const {
        put(*this, u, u);
    }

    template <class E>
    void operator()(on_tree_edge, E e) const {
        put(*this, e.target(), e.source());
    }

    template <class E>
    void operator()(on_edge_relaxed, E e) const {
        operator()(on_tree_edge{}, e);
    }
};

template <class Map>
inline predecessor_recorder<Map> make_predecessor_recorder(Map&& predecessor) {
    return {std::forward<Map>(predecessor)};
}

template <class Map, class T>
inline vertex_initializer<predecessor_recorder<Map>> make_predecessor_recorder(Map&& predecessor,
                                                                               T     value) {
    return {{std::forward<Map>(predecessor)}, value};
}

template <class Graph>
inline auto default_predecessor_recorder(const Graph& graph) {
    return make_predecessor_recorder(default_vertex_property_map<typename Graph::vertex>(graph),
                                     graph.null_vertex());
}

}  // namespace ac
