// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/discovered_flag.hpp>
#include <actl/iterator/output_type.hpp>

namespace ac {

// For binary tree, the output is standard. Otherwise, a vertex is printed between each pair of
// children or once after them if there are less than 2 children.
// The output has from n (for binary tree) to 2 n - 3 (for star graph) vertices.
template <class VertexOutIter, class VertexStateMap>
struct inorder_printer {
    VertexOutIter dst;
    VertexStateMap state;

    using vertex = output_type_t<VertexOutIter>;

    void operator()(on_vertex_initialize, vertex u) {
        put(state, u, -1);
    }
    void operator()(on_vertex_discover, vertex u) {
        put(state, u, 0);
    }
    bool operator()(is_vertex_discovered, vertex u) {
        return get(state, u) != -1;
    }

    template <class E>
    void operator()(on_tree_edge_start, E e) {
        vertex u = e.source();
        decltype(auto) current = get(state, u);
        if (current > 0)
            *dst++ = u;
        if (current < 2)
            put(state, u, current + 1);
    }

    void operator()(on_vertex_finish, vertex u) {
        if (get(state, u) < 2)
            *dst++ = u;
    }
};

template <class Event, class VertexOutIter>
struct vertex_printer {
    VertexOutIter dst;

    void operator()(Event, output_type_t<VertexOutIter> u) {
        *dst++ = u;
    }
};

template <class Graph, class VertexOutIter>
void inorder(const Graph& graph, vertex_t<Graph> s, VertexOutIter dst) {
    auto state = make_default_vertex_map<char>(graph);
    depth_first_search{inorder_printer<VertexOutIter, decltype(state)>{dst, std::move(state)}}(
        graph, s);
}

template <class Graph, class VertexOutIter>
void postorder(const Graph& graph, vertex_t<Graph> s, VertexOutIter dst) {
    depth_first_search{
        vertex_printer<on_vertex_finish, VertexOutIter>{dst}, make_default_discovered_flag(graph)}(
        graph, s);
}

template <class Graph, class VertexOutIter>
void preorder(const Graph& graph, vertex_t<Graph> s, VertexOutIter dst) {
    depth_first_search{
        vertex_printer<on_vertex_start, VertexOutIter>{dst}, make_default_discovered_flag(graph)}(
        graph, s);
}

/// Outputs topological sort of the DAG in reverse order.
template <class Graph, class VertexOutIter>
void topological_sort(const Graph& graph, VertexOutIter dst) {
    depth_first_search{
        vertex_printer<on_vertex_finish, VertexOutIter>{dst},
        make_default_discovered_flag(graph)}(graph);
}

} // namespace ac
