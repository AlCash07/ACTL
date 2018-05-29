/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/adj_list_traits.hpp>

namespace ac {

namespace detail {

template <class Dir, class OEC, class EC, class VC, class T = value_type_t<VC>>
class adj_list_vertices : public adj_list_vertices<Dir, OEC, EC, VC, none> {
    using base_t = adj_list_vertices<Dir, OEC, EC, VC, none>;

public:
    using vertex_id = typename base_t::vertex_id;

    using base_t::base_t;

    auto operator[](vertex_property) {
        return detail::append_bundle_property_map(
            make_generic_container_property_map(this->vertices_));
    }

    auto operator[](vertex_property) const {
        return detail::append_bundle_property_map(
            make_generic_container_property_map(this->vertices_));
    }

    T&       operator[](vertex_id v)       { return get((*this)[vertex_property{}], v); }
    const T& operator[](vertex_id v) const { return get((*this)[vertex_property{}], v); }
};

template <class Dir, class OEC, class EC, class VC>
class adj_list_vertices<Dir, OEC, EC, VC, none>
    : public adj_list_traits<Dir, OEC, EC, VC>::vertices_base {
    using base_t = typename adj_list_traits<Dir, OEC, EC, VC>::vertices_base;

public:
    using base_t::base_t;
};

// This class is needed to disambiguate operator[] in case vertex_id and edges::edge_id are the
// same.
template <class Dir, class OEC, class EC, class VC, class T>
class adj_list : public adj_list<Dir, OEC, EC, VC, none> {
public:
    using edge_id = typename adj_list<Dir, OEC, EC, VC, none>::edge_id;

    using adj_list<Dir, OEC, EC, VC, none>::operator[];

    auto operator[](edge_property) { return this->edges_[edge_property{}]; }

    auto operator[](edge_property) const { return this->edges_[edge_property{}]; }

    T&       operator[](edge_id e)       { return get((*this)[edge_property{}], e); }
    const T& operator[](edge_id e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class OEC, class EC, class VC>
class adj_list<Dir, OEC, EC, VC, none> : public detail::adj_list_vertices<Dir, OEC, EC, VC> {
    using base_t = detail::adj_list_vertices<Dir, OEC, EC, VC>;
    using traits = detail::adj_list_traits<Dir, OEC, EC, VC>;
    using base_t::vertices_;

public:
    static constexpr bool is_undirected         = std::is_same_v<Dir, undirected>;
    static constexpr bool is_directed           = !is_undirected;
    static constexpr bool is_bidirectional      = std::is_same_v<Dir, bidirectional>;
    static constexpr bool allows_parallel_edges = !is_unique_associative<OEC>::value;

    using directed_category = Dir;

    using vertex_id    = typename base_t::vertex_id;
    using out_edge_id  = typename traits::out_edge_container::id;
    using edge_list_id = typename traits::edges::edge_id;

    struct edge_id {
        vertex_id    v;
        out_edge_id  out_edge;
        edge_list_id edge;

        operator edge_list_id() const { return edge; }
    };

    int edge_count() const { return edges_.edge_count(); }

    // range<edge_iterator> edges() const {
    //    auto edges = edges_.id_range();
    //    return {edge_iterator(edges.begin()), edge_iterator(edges.end())};
    //}

    template <class... Ts>
    std::pair<edge_id, bool> try_add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        auto edge = edges_.null_edge();
        auto& u_edges = vertices_[u].out_edges;
        auto[out_edge, ok] = u_edges.emplace(v, edge);
        if (!ok) {
            return {edge_id{u, out_edge, edge}, false};
        }
        u_edges[out_edge].bundle() = edge = edges_.add_edge(u, v, std::forward<Ts>(args)...);
        if constexpr (is_undirected) {
            vertices_[v].out_edges.emplace(u, edge);
        } else if constexpr (is_bidirectional) {
            vertices_[v].in_edges.emplace(u, edge);
        }
        return {edge_id{u, out_edge, edge}, true};
    }

    template <class... Ts>
    edge_id add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        return try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    void remove_edge(edge_id e) { edges_.erase(e); }

    void clear() {
        base_t::clear();
        edges_.clear();
    }

    void swap(adj_list& other) {
        base_t::swap(other);
        edges_.swap(other.edges_);
    }

private:
    typename traits::edges edges_;
};

template <class Dir, class OEC, class VC, class T>
class adj_list<Dir, OEC, none, VC, T> : public detail::adj_list_vertices<Dir, OEC, none, VC> {
    using base_t = detail::adj_list_vertices<Dir, OEC, none, VC>;

    // Edge bundle is duplicated if graph is undirected or bidirectional, so it's immutable.
    // using out_edge_bundle = std::conditional_t<std::is_same_v<Dir, directed>, bundle, const
    // bundle>;

public:
    static constexpr bool is_undirected         = std::is_same_v<Dir, undirected>;
    static constexpr bool is_directed           = !is_undirected;
    static constexpr bool is_bidirectional      = std::is_same_v<Dir, bidirectional>;
    static constexpr bool allows_parallel_edges = !is_unique_associative<OEC>::value;

    using directed_category = Dir;
};

}  // namespace detail

template <class Directed, class OutEdgeContainer = std::vector<none>, class EdgeContainer = none,
          class VertexContainer = std::vector<none>>
using adjacency_list = detail::adj_list<Directed, OutEdgeContainer, EdgeContainer, VertexContainer,
                                        value_type_t<OutEdgeContainer>>;

}  // namespace ac

namespace std {

template <class Dir, class OEC, class EC, class VC>
struct hash<typename ac::detail::adj_list_vertex_data<Dir, OEC, EC, VC>>
    : hash<typename ac::detail::adj_list_traits<Dir, OEC, EC, VC>::vertex_data> {};

}  // namespace std
