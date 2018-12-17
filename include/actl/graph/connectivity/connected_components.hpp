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

// Should be used only for undirected graphs.
template <class Map>
struct connected_component_recorder : property_map_wrapper_t<Map> {
    using vertex = typename property_traits<Map>::key_type;

    connected_component_recorder(Map&& pm) : property_map_wrapper_t<Map>(std::move(pm)) {}

    void operator()(on_vertex_discover, vertex u) { put(*this, u, count); }
    void operator()(on_search_finish) { ++count; }

    typename property_traits<Map>::value_type count = {};
};

template <class Map, class T>
inline vertex_initializer<connected_component_recorder<Map>> make_connected_component_recorder(
    Map&& component, T value) {
    return {{std::forward<Map>(component)}, value};
}

template <class Graph>
inline auto default_connected_component_recorder(const Graph& graph) {
    return make_connected_component_recorder(default_vertex_property_map<int>(graph), -1);
}

}  // namespace ac
