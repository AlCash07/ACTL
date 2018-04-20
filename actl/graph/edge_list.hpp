/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/edge_list_traits.hpp>
#include <actl/graph/vertex_list.hpp>
#include <actl/property_map/generic_container_property_map.hpp>

namespace ac {

namespace detail {

template <class Dir, class EC, class VC, class S, class T = value_type_t<EC>>
class edge_list_base : public edge_list_base<Dir, EC, VC, S, none> {
    using base_t = edge_list_base<Dir, EC, VC, S, none>;

public:
    using edge_id = typename base_t::edge_id;

    using base_t::base_t;
    using base_t::operator[];

    auto operator[](edge_property) {
        return detail::append_bundle_property_map(
            make_generic_container_property_map(this->edges_));
    }

    auto operator[](edge_property) const {
        return detail::append_bundle_property_map(
            make_generic_container_property_map(this->edges_));
    }

    T&       operator[](edge_id e)       { return get((*this)[edge_property{}], e); }
    const T& operator[](edge_id e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class EC, class VC, class S>
class edge_list_base<Dir, EC, VC, S, none> : public vertex_list<VC> {
    using base_t        = vertex_list<VC>;
    using traits        = detail::edge_list_traits<Dir, EC, typename base_t::vertex_id, S>;
    using edge_vertices = typename traits::vertices;

public:
    using vertex_id = typename base_t::vertex_id;

    using edge_container = typename traits::container;
    using edge_id        = typename detail::edge_id<edge_container, vertex_id>::type;
    using edge_iterator  = typename detail::edge_id<edge_container, vertex_id>::iterator;

    using directed_category = Dir;

    static constexpr bool is_directed   = std::is_same_v<Dir, directed>;
    static constexpr bool is_undirected = !is_directed;

    explicit edge_list_base() = default;

    explicit edge_list_base(int vertices_count) : base_t(vertices_count) {}

    int edge_count() const { return edges_.size(); }

    range<edge_iterator> edges() const {
        auto edges = edges_.id_range();
        return {edges.begin(), edges.end()};
    }

    template <class... Ts>
    std::pair<edge_id, bool> try_add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        if constexpr (is_random_access_v<VC>) {
            vertex_id n = std::max(u, v);
            if (n >= this->vertices_.size()) this->vertices_.resize(n + 1);
        }
        if constexpr (is_undirected && is_associative_v<EC>) {
            if (u > v) std::swap(u, v);
        }
        auto res = edges_.emplace(edge_vertices(u, v), std::forward<Ts>(args)...);
        return {edge_id(res.first), res.second};
    }

    template <class... Ts>
    edge_id add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        return try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    void remove_edge(edge_id e) { edges_.erase(e); }

    // TODO: implement using one pass over edges that will remove edges and decrease int IDs.
    void remove_vertex(vertex_id v);

    void clear() {
        base_t::clear();
        edges_.clear();
    }

    void swap(edge_list_base& other) {
        base_t::swap(other);
        edges_.swap(other.edges_);
    }

protected:
    edge_container edges_;
};

template <class Dir, class EC, class VC, class S>
class edge_list : public edge_list_base<Dir, EC, VC, S> {
public:
    static_assert(!is_associative_v<EC>, "associative edge list requires two vertices");

    using edge_list_base<Dir, EC, VC, S>::edge_list_base;
};

template <class Dir, class EC, class VC>
class edge_list<Dir, EC, VC, two_vertices> : public edge_list_base<Dir, EC, VC, two_vertices> {
    using base_t = edge_list_base<Dir, EC, VC, two_vertices>;

public:
    using vertex_id = typename base_t::vertex_id;
    using edge_id   = typename base_t::edge_id;

    using base_t::base_t;

    vertex_id source(edge_id e) const { return this->edges_[e].u; }

    vertex_id target(edge_id e) const { return this->edges_[e].v; }

    edge_id find_edge(vertex_id u, vertex_id v) const {
        return edge_id(this->edges_.find(typename base_t::edge_vertices(u, v)));
    }
};

}  // namespace detail

template <class Directed,
          class EdgeContainer   = std::vector<none>,
          class VertexContainer = none>
using edge_list = detail::edge_list<Directed, EdgeContainer, VertexContainer, two_vertices>;

}  // namespace ac
