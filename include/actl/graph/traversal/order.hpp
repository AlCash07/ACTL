// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/discovered_flag.hpp>
#include <actl/range/iterator/traits/output_type.hpp>

namespace ac {

// For binary tree, the output is standard. Otherwise, a vertex is printed
// between each pair of children or once after them if there are less than 2
// children. The output has from n (for binary tree) to 2 n - 3 (for star graph)
// vertices.
template<typename VertexOutIter, typename VertexStateMap>
struct inorder_printer {
    VertexOutIter output;
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

    template<typename E>
    void operator()(on_tree_edge_start, E e) {
        vertex u = e.source();
        decltype(auto) current = get(state, u);
        if (current > 0)
            *output++ = u;
        if (current < 2)
            put(state, u, current + 1);
    }

    void operator()(on_vertex_finish, vertex u) {
        if (get(state, u) < 2)
            *output++ = u;
    }
};

template<typename Event, typename VertexOutIter>
struct vertex_printer {
    VertexOutIter output;

    void operator()(Event, output_type_t<VertexOutIter> u) {
        *output++ = u;
    }
};

template<typename Graph, typename VertexOutIter>
void inorder(Graph const& graph, vertex_t<Graph> s, VertexOutIter output) {
    auto state = make_default_vertex_map<char>(graph);
    depth_first_search{inorder_printer<VertexOutIter, decltype(state)>{
        output, std::move(state)
    }}(graph, s);
}

template<typename Graph, typename VertexOutIter>
void postorder(Graph const& graph, vertex_t<Graph> s, VertexOutIter output) {
    depth_first_search{
        vertex_printer<on_vertex_finish, VertexOutIter>{output},
        make_default_discovered_flag(graph)
    }(graph, s);
}

template<typename Graph, typename VertexOutIter>
void preorder(Graph const& graph, vertex_t<Graph> s, VertexOutIter output) {
    depth_first_search{
        vertex_printer<on_vertex_start, VertexOutIter>{output},
        make_default_discovered_flag(graph)
    }(graph, s);
}

/// Outputs topological sort of the DAG in reverse order.
template<typename Graph, typename VertexOutIter>
void topological_sort(Graph const& graph, VertexOutIter output) {
    depth_first_search{
        vertex_printer<on_vertex_finish, VertexOutIter>{output},
        make_default_discovered_flag(graph)
    }(graph);
}

} // namespace ac
