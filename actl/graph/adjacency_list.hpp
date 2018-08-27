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
    using base_t = adj_list_edges<Dir, OEC, EC, VC, S, none>;

public:
    using edge = typename base_t::edge;

    using base_t::base_t;
    using base_t::operator[];

    auto operator[](edge_property) { return this->edge_list_[edge_property{}]; }

    auto operator[](edge_property) const { return this->edge_list_[edge_property{}]; }

    T&       operator[](edge e) { return get((*this)[edge_property{}], e); }
    const T& operator[](edge e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class OEC, class EC, class VC, class S>
class adj_list_edges<Dir, OEC, EC, VC, S, none> : public adj_list_vertices<Dir, OEC, EC, VC> {
protected:
    using base_t = adj_list_vertices<Dir, OEC, EC, VC>;
    using traits = adj_list_traits<Dir, OEC, EC, VC>;
    using out_it = typename traits::out_edge_container::const_iterator;
    using vertex = typename base_t::vertex;
    using edge   = typename traits::edge_list::edge;
    using base_t::vertices_;

    out_it out_begin(vertex u) const { return vertices_[u].first().out_edges.begin(); }
    out_it out_end(vertex u) const { return vertices_[u].first().out_edges.end(); }

    edge get_edge(vertex u, const typename traits::out_edge_data& oed) const {
        auto e = oed.second();
        if constexpr (std::is_same_v<typename traits::out_edge_data::first_type, none>) {
            return edge_list_.get_edge(u, e);
        } else {
            return edge(u, oed.first(), e);
        }
    }

    typename traits::edge_list edge_list_;

public:
    static_assert(!is_associative_v<EC>,
                  "use std::list instead of associative container for adjacency list");

    using base_t::base_t;
    using base_t::operator[];

    template <class... Ts>
    std::pair<edge, bool> try_add_edge(vertex u, vertex v, Ts&&... args) {
        typename traits::edge_list::edge_id e;
        auto& u_edges = vertices_[u].first().out_edges;
        auto[out_edge, ok] = u_edges.emplace(v, e);
        if (!ok) {
            return {edge(), false};
        }
        u_edges[out_edge].second() = e = edge_list_.add_edge(u, v, std::forward<Ts>(args)...);
        if constexpr (base_t::is_undirected) {
            vertices_[v].first().out_edges.emplace(u, e);
        } else if constexpr (base_t::is_bidirectional) {
            vertices_[v].first().in_edges.emplace(u, e);
        }
        return {edge(u, v, e), true};
    }

    void_property_map<edge> operator[](edge_property) const { return {}; }

    void operator[](edge) const {}
};

/* Adjacency list without edge container */

template <class Dir, class OEC, class EC, class VC, class T>
class adj_list_edges<Dir, OEC, EC, VC, none, T>
    : public adj_list_edges<Dir, OEC, EC, VC, none, none> {
    using base_t    = adj_list_edges<Dir, OEC, EC, VC, none, none>;
    using reference = add_const_if_t<base_t::is_undirected, T&>;

public:
    using edge = typename base_t::edge;

    template <bool Const, class Ref = add_const_if_t<Const, reference>>
    class edge_property_map : public property_map<edge, T, Ref, false, false, !Const> {
        using vertices_ref = add_const_if_t<Const, typename base_t::vertex_container&>;

        vertices_ref vertices_;

    public:
        edge_property_map(vertices_ref& vertices) : vertices_{vertices} {}

        friend Ref get(const edge_property_map& pm, edge key) {
            return pm.vertices_[key.source()].first().out_edges[key].second();
        }
    };

    using base_t::base_t;
    using base_t::operator[];

    edge_property_map<false> operator[](edge_property) { return {this->vertices_}; }
    edge_property_map<true>  operator[](edge_property) const { return {this->vertices_}; }

    reference operator[](edge e) { return get((*this)[edge_property{}], e); }
    const T&  operator[](edge e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class OEC, class EC, class VC>
class adj_list_edges<Dir, OEC, EC, VC, none, none> : public adj_list_vertices<Dir, OEC, EC, VC> {
protected:
    using base_t = adj_list_vertices<Dir, OEC, EC, VC>;
    using traits = adj_list_traits<Dir, OEC, EC, VC>;
    using out_id = typename traits::out_edge_container::id;
    using out_it = typename traits::out_edge_container::id_iterator;
    using vertex = typename base_t::vertex;
    using edge   = edge<vertex, out_id, true>;
    using base_t::vertices_;

    out_it out_begin(vertex u) const { return vertices_[u].first().out_edges.id_range().begin(); }
    out_it out_end(vertex u) const { return vertices_[u].first().out_edges.id_range().end(); }

    edge get_edge(vertex u, out_id oe) const {
        return edge(u, vertices_[u].first().out_edges[oe].first(), oe);
    }

    edge get_edge(vertex u, const typename traits::in_edge_data& ied) const {
        return edge(ied.first(), u, ied.second());
    }

    typename traits::edge_list edge_list_;

public:
    using base_t::base_t;
    using base_t::operator[];

    template <class... Ts>
    std::pair<edge, bool> try_add_edge(vertex u, vertex v, Ts&&... args) {
        auto& u_edges = vertices_[u].first().out_edges;
        auto[out_edge, ok] = u_edges.emplace(v, std::forward<Ts>(args)...);
        if (!ok) {
            return {edge(), false};
        }
        edge_list_.add_edge(u, v);
        if constexpr (base_t::is_undirected) {
            vertices_[v].first().out_edges.emplace(u, std::forward<Ts>(args)...);
        } else if constexpr (base_t::is_bidirectional) {
            vertices_[v].first().in_edges.emplace(u, out_edge);
        }
        return {edge(u, v, out_edge), true};
    }

    void_property_map<edge> operator[](edge_property) const { return {}; }

    void operator[](edge) const {}
};

}  // namespace detail

/* Common functionality */

template <class Dir, class OEC, class EC, class VC>
class adjacency_list : public detail::adj_list_edges<Dir, OEC, EC, VC> {
    using base_t = detail::adj_list_edges<Dir, OEC, EC, VC>;
    using traits = typename base_t::traits;
    using base_t::edge_list_;

    template <class AL, class S>
    friend struct detail::edge_it;

public:
    using vertex = typename base_t::vertex;
    using edge   = typename base_t::edge;

    using edge_selector     = value_type_t<EC>;
    using edge_iterator     = typename detail::edge_it<adjacency_list>::type;
    using out_edge_iterator = detail::adj_list_out_edge_it<adjacency_list, typename base_t::out_it>;
    using in_edge_iterator =
        std::conditional_t<std::is_same_v<edge_selector, none> && base_t::is_bidirectional,
                           detail::adj_list_out_edge_it<
                               adjacency_list, typename traits::in_edge_container::const_iterator>,
                           detail::reverse_edge_it<out_edge_iterator>>;

    using base_t::base_t;

    int edge_count() const { return edge_list_.edge_count(); }

    range<edge_iterator> edges() const {
        if constexpr (std::is_same_v<edge_selector, two_vertices>) {
            return edge_list_.edges();
        } else {
            return {edge_iterator(this, true), edge_iterator(this, false)};
        }
    }

    range<out_edge_iterator> out_edges(vertex u) const {
        return {out_edge_iterator(this, u, this->out_begin(u)),
                out_edge_iterator(this, u, this->out_end(u))};
    }

    template <bool D = std::is_same_v<Dir, directed>>
    std::enable_if_t<!D, range<in_edge_iterator>> in_edges(vertex u) const {
        if constexpr (base_t::is_undirected) {
            auto out = out_edges(u);
            return {in_edge_iterator(out.begin()), in_edge_iterator(out.end())};
        } else {
            auto in_begin = this->vertices_[u].first().in_edges.begin();
            auto in_end   = this->vertices_[u].first().in_edges.end();
            if constexpr (std::is_same_v<edge_selector, none>) {
                return {in_edge_iterator(this, u, in_begin), in_edge_iterator(this, u, in_end)};
            } else {
                return {in_edge_iterator(out_edge_iterator(this, u, in_begin)),
                        in_edge_iterator(out_edge_iterator(this, u, in_end))};
            }
        }
    }

    template <class... Ts>
    edge add_edge(vertex u, vertex v, Ts&&... args) {
        return this->try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    template <class... Ts, bool UA = is_unique_associative<VC>::value, class T = value_type_t<VC>>
    std::enable_if_t<UA, edge> add_edge(const T& u, const T& v, Ts&&... args) {
        return add_edge(add_vertex(u), add_vertex(v), std::forward<Ts>(args)...);
    }

    void clear() {
        base_t::clear();
        edge_list_.clear();
    }

    void swap(adjacency_list& other) {
        detail::adj_list_vertices<Dir, OEC, EC, VC>::swap(other);
        edge_list_.swap(other.edge_list_);
    }

    friend edge_iterator;
    friend out_edge_iterator;
    friend in_edge_iterator;
};

}  // namespace ac
