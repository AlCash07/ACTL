/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/adj_list_iterators.hpp>
#include <actl/graph/detail/adj_list_vertices.hpp>

namespace ac {

template <class Directed,
          class OutEdgeContainer = std::vector<none>,
          class EdgeContainer    = none,
          class VertexContainer  = std::vector<none>>
class adjacency_list;

namespace detail {

/* Adjacency list with edge container */

template <class Dir, class OEC, class EC, class VC, class S = value_type_t<EC>,
          class T = value_type_t<OEC>>
class adj_list_edges : public adj_list_edges<Dir, OEC, EC, VC, S, none> {
public:
    using edge_id = typename adj_list_edges<Dir, OEC, EC, VC, S, none>::edge_id;

    using adj_list_edges<Dir, OEC, EC, VC, S, none>::operator[];

    auto operator[](edge_property) { return this->edges_[edge_property{}]; }

    auto operator[](edge_property) const { return this->edges_[edge_property{}]; }

    T&       operator[](edge_id e)       { return get((*this)[edge_property{}], e); }
    const T& operator[](edge_id e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class OEC, class EC, class VC, class S>
class adj_list_edges<Dir, OEC, EC, VC, S, none> : public adj_list_vertices<Dir, OEC, EC, VC> {
    using base_t = adj_list_vertices<Dir, OEC, EC, VC>;
    using traits = adj_list_traits<Dir, OEC, EC, VC>;
    using base_t::vertices_;

public:
    static_assert(!is_associative_v<EC>,
                  "use std::list instead of associative container for adjacency list");

    using vertex_id    = typename base_t::vertex_id;
    using out_edge_id  = typename traits::out_edge_container::id;
    using edge_list_id = typename traits::edges::edge_id;
    using edge_id      = typename adj_list_edge_id<S, vertex_id, edge_list_id>::type;

    template <class... Ts>
    std::pair<edge_id, bool> try_add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        auto edge = edges_.null_edge();
        auto& u_edges = vertices_[u].out_edges;
        auto[out_edge, ok] = u_edges.emplace(v, edge);
        if (!ok) {
            return {edge_id{}, false};
        }
        u_edges[out_edge].bundle() = edge = edges_.add_edge(u, v, std::forward<Ts>(args)...);
        if constexpr (base_t::is_undirected) {
            vertices_[v].out_edges.emplace(u, edge);
        } else if constexpr (base_t::is_bidirectional) {
            vertices_[v].in_edges.emplace(u, edge);
        }
        return {get_edge_id(u, u_edges[out_edge]), true};
    }

protected:
    using edges  = typename traits::edges;
    using out_it = typename traits::out_edge_container::const_iterator;

    out_it out_begin(vertex_id u) const { return vertices_[u].out_edges.begin(); }
    out_it out_end(vertex_id u)   const { return vertices_[u].out_edges.end(); }

    out_it in_begin(vertex_id u) const { return vertices_[u].in_edges.begin(); }
    out_it in_end(vertex_id u)   const { return vertices_[u].in_edges.end(); }

    static edge_id get_edge_id(vertex_id u, const typename traits::out_edge_data& oe) {
        auto e = oe.bundle();
        if constexpr (std::is_same_v<S, two_vertices>) {
            return e;
        } else if constexpr (std::is_same_v<S, one_vertex>) {
            return {u, e};
        } else {
            return {u, (vertex_id)oe, e};
        }
    }

    edges edges_;
};

/* Adjacency list without edge container */

template <class Dir, class OEC, class EC, class VC, class T>
class adj_list_edges<Dir, OEC, EC, VC, none, T>
    : public adj_list_edges<Dir, OEC, EC, VC, none, none> {
public:
    using edge_id = typename adj_list_edges<Dir, OEC, EC, VC, none, none>::edge_id;

    using adj_list_edges<Dir, OEC, EC, VC, none, none>::operator[];

    // fix
    //    auto operator[](edge_property) { return this->edges_[edge_property{}]; }
    //
    //    auto operator[](edge_property) const { return this->edges_[edge_property{}]; }

    T&       operator[](edge_id e)       { return get((*this)[edge_property{}], e); }
    const T& operator[](edge_id e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class OEC, class EC, class VC>
class adj_list_edges<Dir, OEC, EC, VC, none, none> : public adj_list_vertices<Dir, OEC, EC, VC> {
    using base_t = adj_list_vertices<Dir, OEC, EC, VC>;
    using traits = adj_list_traits<Dir, OEC, EC, VC>;
    using base_t::vertices_;

public:
    using vertex_id   = typename base_t::vertex_id;
    using out_edge_id = typename traits::out_edge_container::id;

    struct edge_id {
        vertex_id   u;
        out_edge_id e;
    };

    template <class... Ts>
    std::pair<edge_id, bool> try_add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        auto& u_edges = vertices_[u].out_edges;
        auto[out_edge, ok] = u_edges.emplace(v, std::forward<Ts>(args)...);
        if (!ok) {
            return {edge_id{}, false};
        }
        edges_.add_edge(u, v);
        if constexpr (base_t::is_undirected) {
            vertices_[v].out_edges.emplace(u, std::forward<Ts>(args)...);
        } else if constexpr (base_t::is_bidirectional) {
            vertices_[v].in_edges.emplace(u, std::forward<Ts>(args)...);
        }
        return {edge_id{u, out_edge}, true};
    }

protected:
    using edges  = typename traits::edges;
    using out_it = typename traits::out_edge_container::id_iterator;

    out_it out_begin(vertex_id u) const { return vertices_[u].out_edges.id_begin(); }
    out_it out_end(vertex_id u)   const { return vertices_[u].out_edges.id_end(); }

    out_it in_begin(vertex_id u) const { return vertices_[u].in_edges.id_begin(); }
    out_it in_end(vertex_id u)   const { return vertices_[u].in_edges.id_end(); }

    static edge_id get_edge_id(vertex_id u, out_edge_id e) { return {u, e}; }

    edges edges_;
};

}  // namespace detail

template <class Dir, class OEC, class EC, class VC>
class adjacency_list : public detail::adj_list_edges<Dir, OEC, EC, VC> {
    using base_t = detail::adj_list_edges<Dir, OEC, EC, VC>;

    static constexpr bool use_edges_it = std::is_same_v<value_type_t<EC>, two_vertices>;

public:
    using vertex_id = typename base_t::vertex_id;
    using edge_id   = typename base_t::edge_id;

    using edge_iterator = std::conditional_t<use_edges_it, typename base_t::edges::edge_iterator,
                                             detail::adj_list_edge_it<adjacency_list>>;

    using out_edge_iterator = detail::adj_list_out_edge_it<adjacency_list>;

    friend edge_iterator;
    friend out_edge_iterator;

    using base_t::base_t;

    int edge_count() const { return this->edges_.edge_count(); }

    range<edge_iterator> edges() const {
        if constexpr (use_edges_it) {
            return this->edges_.edges();
        } else {
            return {edge_iterator(this, true), edge_iterator(this, false)};
        }
    }

    range<out_edge_iterator> out_edges(vertex_id u) const {
        return {out_edge_iterator(u, this->out_begin(u)), out_edge_iterator(u, this->out_end(u))};
    }

    template <bool D = std::is_same_v<Dir, directed>>
    std::enable_if_t<!D, range<out_edge_iterator>> in_edges(vertex_id u) const {
        if constexpr (base_t::is_undirected) {
            return out_edges(u);
        } else {
            return {out_edge_iterator(u, this->in_begin(u)), out_edge_iterator(u, this->in_end(u))};
        }
    }

    template <class... Ts>
    edge_id add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        return this->try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    template <class... Ts, bool UA = is_unique_associative<VC>::value, class T = value_type_t<VC>>
    std::enable_if_t<UA, edge_id> add_edge(const T& u, const T& v, Ts&&... args) {
        return add_edge(add_vertex(u), add_vertex(v), std::forward<Ts>(args)...);
    }

    void clear() {
        base_t::clear();
        this->edges_.clear();
    }

    void swap(adjacency_list& other) {
        detail::adj_list_vertices<Dir, OEC, EC, VC>::swap(other);
        this->edges_.swap(other.edges_);
    }
};

}  // namespace ac
