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
    using typename base_t::edge;

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
    using edge   = typename traits::edges::edge;
    using typename base_t::vertex;
    using base_t::outs;

    out_it out_begin(vertex u) const { return outs(u).begin(); }
    out_it out_end(vertex u) const { return outs(u).end(); }

    edge get_edge(vertex u, const typename traits::out_edge_data& oed) const {
        auto e = oed.second();
        if constexpr (std::is_same_v<typename traits::out_edge_data::first_type, none>) {
            return edge_list_.get_edge(u, e);
        } else {
            return edge(u, oed.first(), e);
        }
    }

    edge get_edge(vertex u, container_id<typename traits::out_edge_container> oe) const {
        return get_edge(u, id_at(outs(u), oe));
    }

    template <class... Ts>
    std::pair<edge, bool> try_add_edge_impl(vertex u, vertex v, Ts&&... args) {
        typename traits::edges::edge_id e;
        auto& u_edges = outs(u);
        auto[out_edge, ok] = id_emplace(u_edges, v, e);
        if (!ok) {
            return {edge(), false};
        }
        e = edge_list_.add_edge(u, v, std::forward<Ts>(args)...);
        id_at(u_edges, out_edge).second() = e;
        if constexpr (base_t::is_undirected) {
            if (u != v) id_emplace(outs(v), u, e);
        } else if constexpr (base_t::is_bidirectional) {
            id_emplace(this->ins(v), u, e);
        }
        return {edge(u, v, e), true};
    }

    typename traits::edges edge_list_;

public:
    static_assert(!is_associative_container_v<EC>,
                  "use std::list instead of associative container for adjacency list");

    using edge_container = typename traits::edges::edge_container;

    using base_t::base_t;
    using base_t::operator[];

    dummy_property_map<edge> operator[](edge_property) const { return {}; }

    void operator[](edge) const {}
};

/* Adjacency list without edge container */

template <class Dir, class OEC, class EC, class VC, class T>
class adj_list_edges<Dir, OEC, EC, VC, none, T>
    : public adj_list_edges<Dir, OEC, EC, VC, none, none> {
    using base_t    = adj_list_edges<Dir, OEC, EC, VC, none, none>;
    using reference = add_const_if_t<base_t::is_undirected, T&>;

public:
    using typename base_t::edge;

    template <bool Const, class Ref = add_const_if_t<Const, reference>>
    class edge_property_map : public property_map<edge, T, Ref, false, false, !Const> {
        using vertices_ref = add_const_if_t<Const, typename base_t::vertex_container&>;

        vertices_ref vertices_;

    public:
        edge_property_map(vertices_ref& vertices) : vertices_{vertices} {}

        friend Ref get(const edge_property_map& pm, edge e) {
            return id_at(id_at(pm.vertices_, e.source()).first().out_edges, e.bundle()).second();
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
    using out_id = container_id<typename traits::out_edge_container>;
    using out_it = container_id_iterator<typename traits::out_edge_container>;
    using typename base_t::vertex;
    using edge   = edge<vertex, out_id, true>;
    using base_t::outs;

    out_it out_begin(vertex u) const { return id_range(outs(u)).begin(); }
    out_it out_end(vertex u) const { return id_range(outs(u)).end(); }

    edge get_edge(vertex u, out_id oe) const { return edge(u, id_at(outs(u), oe).first(), oe); }

    edge get_edge(vertex u, const typename traits::in_edge_data& ied) const {
        return edge(ied.first(), u, ied.second());
    }

    template <class... Ts>
    std::pair<edge, bool> try_add_edge_impl(vertex u, vertex v, Ts&&... args) {
        auto& u_edges = outs(u);
        auto[out_edge, ok] = id_emplace(u_edges, v, std::forward<Ts>(args)...);
        if (!ok) {
            return {edge(), false};
        }
        edge_list_.add_edge(u, v);
        if constexpr (base_t::is_undirected) {
            if (u != v) id_emplace(outs(v), u, std::forward<Ts>(args)...);
        } else if constexpr (base_t::is_bidirectional) {
            id_emplace(this->ins(v), u, out_edge);
        }
        return {edge(u, v, out_edge), true};
    }

    typename traits::edges edge_list_;

public:
    using edge_container = none;

    using base_t::base_t;
    using base_t::operator[];

    dummy_property_map<edge> operator[](edge_property) const { return {}; }

    void operator[](edge) const {}
};

}  // namespace detail

/* Common functionality */

template <class Dir, class OEC, class EC, class VC>
class adjacency_list : public detail::adj_list_edges<Dir, OEC, EC, VC> {
    using base_t = detail::adj_list_edges<Dir, OEC, EC, VC>;
    using typename base_t::traits;
    using base_t::edge_list_;

    template <class AL, class S>
    friend struct detail::edge_it;

public:
    using typename base_t::vertex;
    using typename base_t::edge;

    using edge_selector     = value_type_t<EC>;
    using edge_iterator     = typename detail::edge_it<adjacency_list>::type;
    using out_edge_id       = typename base_t::out_it;
    using out_edge_iterator = detail::adj_list_out_edge_it<adjacency_list, out_edge_id>;
    using in_edge_iterator =
        std::conditional_t<std::is_same_v<edge_selector, none> && base_t::is_bidirectional,
                           detail::adj_list_out_edge_it<
                               adjacency_list, typename traits::in_edge_container::const_iterator>,
                           detail::reverse_edge_it<out_edge_iterator>>;

    using base_t::base_t;

    int edge_count() const { return edge_list_.edge_count(); }

    int degree(vertex u) const { return out_edges(u).size(); }

    iterator_range<edge_iterator> edges() const {
        if constexpr (std::is_same_v<edge_selector, two_vertices>) {
            return edge_list_.edges();
        } else {
            return {edge_iterator(this, true), edge_iterator(this, false)};
        }
    }

    iterator_range<out_edge_iterator> out_edges(vertex u) const {
        return {out_edge_iterator(this, u, this->out_begin(u)),
                out_edge_iterator(this, u, this->out_end(u))};
    }

    template <bool D = std::is_same_v<Dir, directed>>
    std::enable_if_t<!D, iterator_range<in_edge_iterator>> in_edges(vertex u) const {
        if constexpr (base_t::is_undirected) {
            auto out = out_edges(u);
            return {in_edge_iterator(out.begin()), in_edge_iterator(out.end())};
        } else {
            auto in_begin = this->ins(u).begin();
            auto in_end   = this->ins(u).end();
            if constexpr (std::is_same_v<edge_selector, none>) {
                return {in_edge_iterator(this, u, in_begin), in_edge_iterator(this, u, in_end)};
            } else {
                return {in_edge_iterator(out_edge_iterator(this, u, in_begin)),
                        in_edge_iterator(out_edge_iterator(this, u, in_end))};
            }
        }
    }

    template <class... Ts>
    std::pair<edge, bool> try_add_edge(vertex u, vertex v, Ts&&... args) {
        if constexpr (is_random_access_container_v<VC>) {
            vertex n = std::max(u, v);
            if (n >= this->vertices_.size()) this->vertices_.resize(n + 1);
        }
        return this->try_add_edge_impl(u, v, std::forward<Ts>(args)...);
    }

    template <class... Ts>
    edge add_edge(vertex u, vertex v, Ts&&... args) {
        return try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    template <class... Ts, bool UA = is_unique_associative_container_v<VC>,
              class T = value_type_t<VC>>
    std::enable_if_t<UA, edge> add_edge(const T& u, const T& v, Ts&&... args) {
        return add_edge(add_vertex(u), add_vertex(v), std::forward<Ts>(args)...);
    }

    edge find_edge(vertex u, vertex v) const {
        if constexpr (traits::has_out_vertex) {
            return this->get_edge(u, id_find(this->outs(u), v));
        } else {
            for (auto oe : out_edges(u)) {
                if (oe.target() == v) return oe;
            }
            return edge();
        }
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
