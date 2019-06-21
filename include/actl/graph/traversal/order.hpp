/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/discovered_flag.hpp>
#include <actl/traits/output_type.hpp>

namespace ac {

// For binary tree, the output is standard. Otherwise, a vertex is printer between each pair of
// children or once after them if there are less than 2 children.
// The output has from n (for binary tree) to 2 n - 3 (for star graph) vertices.
template <class VertexOutputIterator, class VertexStateMap>
struct inorder_printer {
    VertexOutputIterator it;
    VertexStateMap       state;

    using vertex = output_type_t<VertexOutputIterator>;

    void operator()(on_vertex_initialize, vertex u) { put(state, u, -1); }
    void operator()(on_vertex_discover, vertex u) { put(state, u, 0); }
    bool operator()(is_vertex_discovered, vertex u) { return get(state, u) != -1; }

    template <class E>
    void operator()(on_tree_edge, E e) {
        vertex u = e.source();
        auto current = get(state, u);
        if (current > 0) *it++= u;
        if (current < 2) put(state, u, current + 1);
    }

    void operator()(on_vertex_finish, vertex u) {
        if (get(state, u) < 2) *it++ = u;
    }
};

template <class Event, class VertexOutputIterator>
struct vertex_printer {
    VertexOutputIterator it;

    void operator()(Event, output_type_t<VertexOutputIterator> u) { *it++ = u; }
};

template <class Graph, class VertexOutputIterator>
inline void print_inorder(const Graph& graph, typename Graph::vertex s, VertexOutputIterator it) {
    auto state = default_vertex_property_map<char>(graph);
    depth_first_search{
        inorder_printer<VertexOutputIterator, decltype(state)>{it, std::move(state)}}(graph, s);
}

template <class Graph, class VertexOutputIterator>
inline void print_postorder(const Graph& graph, typename Graph::vertex s, VertexOutputIterator it) {
    depth_first_search{vertex_printer<on_vertex_finish, VertexOutputIterator>{it},
                       default_discovered_flag(graph)}(graph, s);
}

template <class Graph, class VertexOutputIterator>
inline void print_preorder(const Graph& graph, typename Graph::vertex s, VertexOutputIterator it) {
    depth_first_search{vertex_printer<on_vertex_examine, VertexOutputIterator>{it},
                       default_discovered_flag(graph)}(graph, s);
}

/**
 * Outputs topological sort of the DAG in reverse order.
 */
template <class Graph, class VertexOutputIterator>
inline void topological_sort(const Graph& graph, VertexOutputIterator it) {
    depth_first_search{vertex_printer<on_vertex_finish, VertexOutputIterator>{it},
                       default_discovered_flag(graph)}(graph);
}

}  // namespace ac
