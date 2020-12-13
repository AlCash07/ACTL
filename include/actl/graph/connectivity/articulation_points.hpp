// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/graph/connectivity/detail/component_stack.hpp>
#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/map/dummy_map.hpp>

namespace ac {

template <class Graph, class ArticulationMap, class ComponentStack, class TimeMap, class LowMap>
struct articulation_point_finder {
    static_assert(Graph::is_undirected);

    using V = vertex_t<Graph>;
    using E = edge_t<Graph>;
    using T = map_value_t<TimeMap>;

    void operator()(on_vertex_initialize, V u) { put(time, u, 0); }
    bool operator()(is_vertex_discovered, V u) { return get(time, u) != 0; }

    void operator()(on_vertex_start, V u) {
        put(time, u, ++time_now);
        put(low, u, time_now);
    }

    void operator()(on_search_start, V u) {
        root = u;
        root_children = 0;
    }

    void operator()(on_non_tree_edge, E e) {
        V u = e.source();
        V v = e.target();
        T vtime = get(time, v);
        if (vtime < get(time, u)) components.push(e);
        if (vtime < get(low, u)) {
            put(low, u, vtime);
        }
    }

    void operator()(on_tree_edge_finish, E e) {
        V u = e.source();
        V v = e.target();
        T utime = get(time, u);
        if (utime <= get(low, v)) {
            if (u == root) {
                ++root_children;
            } else {
                put(articulation_map, u, true);
            }
            components.pop_while([this, utime](E e) {
                return utime <= get(time, e.source()) && utime <= get(time, e.target());
            });
        }
    }

    void operator()(on_search_finish, V u) {
        if (root_children > 1) put(articulation_map, u, true);
    }

    ArticulationMap articulation_map;
    ComponentStack components;
    TimeMap time;
    LowMap low;
    V root = {};
    T time_now = 0;
    T root_children = 0;
};

template <class G, class... Ts>
inline auto get_articulation_point_finder(const G&, Ts&&... args) {
    return articulation_point_finder<G, Ts...>{std::forward<Ts>(args)...};
}

template <class Graph, class ArticulationMap, class ComponentMap>
void find_articulation_points_and_components(const Graph& graph, ArticulationMap&& articulation_map,
                                             ComponentMap&& component_map) {
    auto apf = get_articulation_point_finder(
        graph, std::forward<ArticulationMap>(articulation_map),
        detail::component_stack{std::forward<ComponentMap>(component_map)},
        // Values of the next two maps can be compressed into bits of one int per vertex.
        make_default_vertex_map<int>(graph), make_default_vertex_map<int>(graph));
    depth_first_search{apf}(graph);
}

template <class Graph, class ArticulationMap>
void find_articulation_points(const Graph& graph, ArticulationMap&& map) {
    find_articulation_points_and_components(graph, std::forward<ArticulationMap>(map), dummy_map{});
}

template <class Graph, class ComponentMap>
void find_biconnected_components(const Graph& graph, ComponentMap&& map) {
    find_articulation_points_and_components(graph, dummy_map{}, std::forward<ComponentMap>(map));
}

}  // namespace ac
