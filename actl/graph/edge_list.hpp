/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/edge_list.hpp>

namespace ac {

template <class Directed,
          class EdgeContainer   = std::vector<none>,
          class VertexContainer = std::vector<none>,
          class Selector        = two_vertices>
class edge_list {
public:
    static constexpr bool is_directed   = std::is_same_v<Directed, directed>;
    static constexpr bool is_undirected = !is_directed;

    using directed_category = Directed;

    using vertex_container = generic_container<VertexContainer>;
    using vertex_id        = typename vertex_container::id;
    using vertex_iterator  = typename vertex_container::id_iterator;

private:
    using traits         = detail::edge_list_traits<Directed, EdgeContainer, vertex_id, Selector>;
    using edge_vertices  = typename traits::vertices;
    using edge_container = typename traits::container;

public:
    using edge_id       = wrap_id<typename edge_container::id>;
    using edge_iterator = wrap_id_iterator<typename edge_container::id_iterator>;

    explicit edge_list(int vertices_count = 0) : vertices_(vertices_count) {}

    /* Vertices operations */

    int vertices_count() const { return vertices_.size(); }

    range<vertex_iterator> vertices() const { return vertices_.id_range(); }

    vertex_id nth_vertex(int n) const {
        ACTL_ASSERT(0 <= n && n < vertex_count());
        return *std::next(vertices_.id_range().begin(), n);
    }

    template <class... Ts>
    vertex_id add_vertex(Ts&&... args) {
        return vertices_.emplace(std::forward<Ts>(args)...).first;
    }

    /* Edges operations */

    int edges_count() const { return edges_.size(); }

    range<edge_iterator> edges() const {
        auto edges = edges_.id_range();
        return {edges.begin(), edges.end()};
    }

    std::enable_if_t<std::is_same_v<Selector, two_vertices>, vertex_id> source(edge_id e) const {
        return edges_[e].u;
    }

    std::enable_if_t<std::is_same_v<Selector, two_vertices>, vertex_id> target(edge_id e) const {
        return edges_[e].v;
    }

    edge_id find_edge(vertex_id u, vertex_id v) const { return edges_.find(edge_vertices(u, v)); }

    template <class... Ts>
    std::pair<edge_id, bool> add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        if constexpr (std::is_same_v<Directed, undirected> && is_associative_v<EdgeContainer>) {
            if (u > v) std::swap(u, v);
        }
        return edges_.emplace(edge_vertices(u, v), std::forward<Ts>(args)...);
    }

    /* Global operations */

    void clear() {
        vertices_.clear();
        edges_.clear();
    }

    void swap(edge_list& other) {
        vertices_.swap(other.vertices_);
        edges_.swap(other.edges_);
    }

private:
    vertex_container vertices_;
    edge_container   edges_;
};

}  // namespace ac
