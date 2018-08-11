/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/edge_list_traits.hpp>
#include <actl/property_map/generic_container_property_map.hpp>

namespace ac {

namespace detail {

template <class Dir, class VId, class EC, class S, bool DiffId = true, class T = value_type_t<EC>>
class edge_list_base : public edge_list_base<Dir, VId, EC, S, DiffId, none> {
    using base_t = edge_list_base<Dir, VId, EC, S, DiffId, none>;

public:
    using edge_id = typename base_t::edge_id;

    using base_t::base_t;

    auto operator[](edge_property) {
        return append_bundle_property_map(make_generic_container_property_map(this->edges_));
    }

    auto operator[](edge_property) const {
        return append_bundle_property_map(make_generic_container_property_map(this->edges_));
    }

    T&       operator[](edge_id e)       { return get((*this)[edge_property{}], e); }
    const T& operator[](edge_id e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class VId, class EC, class S, bool DiffId>
class edge_list_base<Dir, VId, EC, S, DiffId, none> {
    using traits         = edge_list_traits<Dir, VId, EC, S>;
    using edge_vertices  = typename traits::vertices;
    using edge_container = typename traits::container;
    using edge_id_impl   = edge_id<VId, edge_container, DiffId>;

public:
    static_assert(std::is_same_v<S, two_vertices> || !is_associative_v<EC>,
                  "associative edge list requires two vertices");

    using edge_id       = typename edge_id_impl::type;
    using edge_iterator = typename edge_id_impl::iterator;

    using directed_category = Dir;

    static constexpr bool is_undirected         = std::is_same_v<Dir, undirected>;
    static constexpr bool is_directed           = !is_undirected;
    static constexpr bool allows_parallel_edges = !is_unique_associative<EC>::value;

    explicit edge_list_base() = default;

    int edge_count() const { return edges_.size(); }

    range<edge_iterator> edges() const {
        auto edges = edges_.id_range();
        return {edge_iterator(edges.begin()), edge_iterator(edges.end())};
    }

    edge_id null_edge() const { return edges_.null_id(); }

    template <class... Ts>
    std::pair<edge_id, bool> try_add_edge(VId u, VId v, Ts&&... args) {
        if constexpr (is_undirected && is_associative_v<EC>) {
            if (u > v) std::swap(u, v);
        }
        auto res = edges_.emplace(edge_vertices(u, v), std::forward<Ts>(args)...);
        return {edge_id(res.first), res.second};
    }

    template <class... Ts>
    edge_id add_edge(VId u, VId v, Ts&&... args) {
        return try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    void remove_edge(edge_id e) { edges_.erase(e); }

    void clear() { edges_.clear(); }

    void swap(edge_list_base& other) { edges_.swap(other.edges_); }

    none operator[](edge_id)       { return none{}; }
    none operator[](edge_id) const { return none{}; }

protected:
    edge_container edges_;
};

}  // namespace detail

template <class Directed, class VertexId, class EdgeContainer = std::vector<none>>
class edge_list : public detail::edge_list_base<Directed, VertexId, EdgeContainer, two_vertices> {
    using base_t = detail::edge_list_base<Directed, VertexId, EdgeContainer, two_vertices>;

public:
    using edge_id = typename base_t::edge_id;

    using base_t::base_t;

    VertexId source(edge_id e) const { return this->edges_[e].u; }

    VertexId target(edge_id e) const { return this->edges_[e].v; }

    edge_id find_edge(VertexId u, VertexId v) const {
        return edge_id(this->edges_.find(typename base_t::edge_vertices(u, v)));
    }

    void swap(edge_list& other) { base_t::swap(other); }
};

}  // namespace ac
