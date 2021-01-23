// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/vertex_initializer.hpp>

namespace ac {

// Should be used only for undirected graphs.
template <class Map>
struct connected_component_recorder {
    using vertex = map_key_t<Map>;

    void operator()(on_vertex_discover, vertex u) {
        put(map, u, count);
    }
    void operator()(on_search_finish, vertex) {
        ++count;
    }

    Map map;
    map_value_t<Map> count = {};
};

template <class Map>
connected_component_recorder(Map&&) -> connected_component_recorder<Map>;

template <class Map, class T>
vertex_initializer<connected_component_recorder<Map>> make_connected_component_recorder(
    Map&& component, T value) {
    return {{std::forward<Map>(component)}, value};
}

template <class Graph>
auto make_default_connected_component_recorder(const Graph& graph) {
    return make_connected_component_recorder(make_default_vertex_map<int>(graph), -1);
}

}  // namespace ac
