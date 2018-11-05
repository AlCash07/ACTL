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
struct distance_recorder : property_map_wrapper_t<Map> {
    void operator()(on_vertex_start, typename property_traits<Map>::key_type u) {
        put(*this, u, 0);
    }

    template <class E>
    void operator()(on_tree_edge, E e) {
        put(*this, e.target(), get(*this, e.source()) + 1);
    }
};

template <class Map>
inline distance_recorder<Map> make_distance_recorder(Map distance) {
    return {distance};
}

template <class Map, class T>
inline vertex_initializer<distance_recorder<Map>> make_distance_recorder(Map&& distance, T value) {
    return {{std::forward<Map>(distance)}, value};
}

template <class Graph>
inline auto default_distance_recorder(const Graph& graph) {
    return make_distance_recorder(make_default_vertex_property_map<int>(graph), -1);
}

}  // namespace ac
