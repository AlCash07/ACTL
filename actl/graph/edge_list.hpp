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
    using edge_id       = typename detail::edge_id<edge_container, vertex_id>::type;
    using edge_iterator = typename detail::edge_id<edge_container, vertex_id>::iterator;

    explicit edge_list() = default;

    explicit edge_list(int vertices_count) : vertices_(vertices_count) {}

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

    template <class S = Selector, bool B = std::is_same_v<S, two_vertices>>
    std::enable_if_t<B, vertex_id> source(edge_id e) const {
        return edges_[e].u;
    }

    template <class S = Selector, bool B = std::is_same_v<S, two_vertices>>
    std::enable_if_t<B, vertex_id> target(edge_id e) const {
        return edges_[e].v;
    }

    template <class S = Selector, bool B = std::is_same_v<S, two_vertices>>
    std::enable_if_t<B, edge_id> find_edge(vertex_id u, vertex_id v) const {
        return edges_.find(edge_vertices(u, v));
    }

    template <class... Ts>
    std::pair<edge_id, bool> add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        if constexpr (is_random_access_v<VertexContainer>) {
            vertex_id n = std::max(u, v);
            if (n >= vertices_.size()) vertices_.resize(n + 1);
        }
        if constexpr (std::is_same_v<Directed, undirected> && is_associative_v<EdgeContainer>) {
            if (u > v) std::swap(u, v);
        }
        return edges_.emplace(edge_vertices(u, v), std::forward<Ts>(args)...);
    }

    /* Global operations */

    //    typename vertex_container::property_map& operator[](vertex_property) { return vertices_; }
    //    const typename vertex_container::property_map& operator[](vertex_property) const {
    //        return vertices_;
    //    }

    //    auto operator[](edge_property) {
    //        return detail::append_bundle_property_map(edges_);
    //    }

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
