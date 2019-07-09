/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/connectivity/detail/component_stack.hpp>
#include <actl/graph/connectivity/detail/once_equal.hpp>
#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/iterator/dummy_output_iterator.hpp>

namespace ac {

template <class Graph, bool ParallelEdges = false>
struct bridge_context : dfs_context<Graph>, detail::once_equal<true, !ParallelEdges> {
    using dfs_context<Graph>::dfs_context;
};

template <class Graph, class BridgeOutIter, class ComponentStack, class TimeMap, class RootMap,
          class DfsStack>
struct bridge_finder {
    static_assert(Graph::is_undirected);

    using V = vertex_t<Graph>;
    using E = edge_t<Graph>;
    using T = map_value_t<TimeMap>;

    void operator()(on_vertex_initialize, V u) { put(time_low, u, 0); }
    bool operator()(is_vertex_discovered, V u) { return get(time_low, u) != 0; }

    void operator()(on_vertex_start, V u) {
        put(time_low, u, ++time);
        components.push(u);
    }

    void operator()(on_non_tree_edge, E e) {
        V v = e.target();
        if (!dfs_stack.empty()) {
            auto& top = dfs_stack.top();
            if (top.vertex() == v && top) return;
        }
        auto low = get(time_low, v);
        V u = e.source();
        if (get(time_low, u) > low) {
            put(time_low, u, low);
            put(not_root, u, true);
        }
    }

    void operator()(on_tree_edge_finish, E e) {
        operator()(on_non_tree_edge{}, e);
        if (!get(not_root, e.target())) *bridges++ = e;
    }

    void operator()(on_vertex_finish, V u) {
        if (!get(not_root, u)) components.pop(u);
    }

    BridgeOutIter bridges;
    ComponentStack components;
    TimeMap time_low;
    RootMap not_root;
    DfsStack dfs_stack;
    T time = {};
};

template <class G, class... Ts>
inline auto get_bridge_finder(const G&, Ts&&... args) {
    return bridge_finder<G, Ts...>{std::forward<Ts>(args)...};
}

template <bool ParallelEdges = true, class Graph, class BridgeOutIter, class ComponentMap>
void find_bridges_and_components(const Graph& graph, BridgeOutIter bridges, ComponentMap&& map) {
    auto bf = get_bridge_finder(
        graph, bridges, detail::component_stack{std::forward<ComponentMap>(map)},
        // Values of the next two maps can be compressed into bits of one int per vertex.
        make_default_vertex_map<int>(graph), make_default_vertex_map<bool>(graph),
        std::stack<bridge_context<Graph, ParallelEdges && Graph::allows_parallel_edges>>{});
    depth_first_search{bf}(graph, bf.dfs_stack);
}

template <bool ParallelEdges = true, class Graph, class BridgeOutIter>
void find_bridges(const Graph& graph, BridgeOutIter bridges) {
    find_bridges_and_components<ParallelEdges>(graph, bridges, dummy_map{});
}

template <bool ParallelEdges = true, class Graph, class ComponentMap>
void find_two_edge_connected_components(const Graph& graph, ComponentMap&& map) {
    find_bridges_and_components<ParallelEdges>(graph, dummy_output_iterator{},
                                               std::forward<ComponentMap>(map));
}

}  // namespace ac
