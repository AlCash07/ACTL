// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/graph/connectivity/detail/component_stack.hpp>
#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>

#pragma once

namespace ac {

template <class Graph, class ComponentStack, class TimeMap, class RootMap>
struct strong_component_finder {
    static_assert(Graph::is_directed);

    using V = vertex_t<Graph>;
    using E = edge_t<Graph>;
    using T = map_value_t<TimeMap>;

    void operator()(on_vertex_initialize, V u) {
        put(time_low, u, 0);
    }
    bool operator()(is_vertex_discovered, V u) {
        return get(time_low, u) != 0;
    }

    void operator()(on_vertex_start, V u) {
        put(time_low, u, ++time_now);
        components.push(u);
    }

    void operator()(on_non_tree_edge, E e) {
        auto low = get(time_low, e.target());
        V u = e.source();
        if (get(time_low, u) > low) {
            put(time_low, u, low);
            put(not_root, u, true);
        }
    }

    void operator()(on_tree_edge_finish, E e) {
        operator()(on_non_tree_edge{}, e);
        put(time_low, e.target(), std::numeric_limits<T>::max());
    }

    void operator()(on_vertex_finish, V u) {
        if (!get(not_root, u))
            components.pop(u);
    }

    void operator()(on_search_finish, V u) {
        put(time_low, u, std::numeric_limits<T>::max());
    }

    ComponentStack components;
    TimeMap time_low;
    RootMap not_root;
    T time_now = 0;
};

template <class G, class... Ts>
auto get_strong_component_finder(const G&, Ts&&... args) {
    return strong_component_finder<G, Ts...>{std::forward<Ts>(args)...};
}

template <class Graph, class ComponentMap>
void find_strong_components(const Graph& graph, ComponentMap&& map) {
    auto scf = get_strong_component_finder(
        graph,
        detail::component_stack{std::forward<ComponentMap>(map)},
        // Values of the next two maps can be compressed into bits of one int per vertex.
        make_default_vertex_map<int>(graph),
        make_default_vertex_map<bool>(graph));
    depth_first_search{scf}(graph);
}

} // namespace ac
