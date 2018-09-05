/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/edge.hpp>
#include <actl/graph/detail/edge_list_traits.hpp>
#include <actl/property_map/dummy_property_map.hpp>
#include <actl/property_map/generic_container_property_map.hpp>
#include <actl/container/dummy_container.hpp>

namespace ac {

namespace detail {

template <class Dir, class V, class EC, class S, class T = value_type_t<EC>>
class edge_list_edges : public edge_list_edges<Dir, V, EC, S, none> {
    using base_t = edge_list_edges<Dir, V, EC, S, none>;

public:
    using edge = typename base_t::edge;

    using base_t::base_t;

    auto operator[](edge_property) {
        return get_second(make_generic_container_property_map(this->edges_));
    }

    auto operator[](edge_property) const {
        return get_second(make_generic_container_property_map(this->edges_));
    }

    T&       operator[](edge e) { return get((*this)[edge_property{}], e); }
    const T& operator[](edge e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class V, class EC, class S>
class edge_list_edges<Dir, V, EC, S, none> {
protected:
    using traits         = edge_list_traits<Dir, V, EC, S>;
    using edge_vertices  = typename traits::vertices;

public:
    static_assert(std::is_same_v<S, two_vertices> || !is_associative_container_v<EC>,
                  "associative edge list requires two vertices");

    using edge_container = typename traits::container;
    using edge_id        = typename edge_container::id;
    using vertex         = V;
    using edge           = edge<V, edge_id>;

    using directed_category = Dir;

    static constexpr bool is_undirected         = std::is_same_v<Dir, undirected>;
    static constexpr bool is_directed           = !is_undirected;
    static constexpr bool allows_parallel_edges = !is_unique_associative_container_v<EC>;

    explicit edge_list_edges() = default;

    int edge_count() const { return edges_.size(); }

    template <class... Ts>
    std::pair<edge, bool> try_add_edge(vertex u, vertex v, Ts&&... args) {
        if constexpr (is_undirected) {
            if (get_id_key(u) > get_id_key(u)) std::swap(u, v);
        }
        auto res = edges_.emplace(edge_vertices(u, v), std::forward<Ts>(args)...);
        return {edge(u, v, res.first), res.second};
    }

    template <class... Ts>
    edge add_edge(vertex u, vertex v, Ts&&... args) {
        return try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    edge get_edge(vertex u, edge_id e) const { return edge(u, edges_[e].first().other(u), e); }

    void clear() { edges_.clear(); }

    void swap(edge_list_edges& other) { edges_.swap(other.edges_); }

    dummy_property_map<edge> operator[](edge_property) const { return {}; }

    void operator[](edge) const {}

protected:
    edge_container edges_;
};

template <class Dir, class V, class EC, class S>
class edge_list_impl : public edge_list_edges<Dir, V, EC, S> {
    using base_t = edge_list_edges<Dir, V, EC, S>;

public:
    using base_t::base_t;

    void swap(edge_list_impl& other) { base_t::swap(other); }
};

template <class Dir, class V, class EC>
class edge_list_impl<Dir, V, EC, two_vertices> : public edge_list_edges<Dir, V, EC, two_vertices> {
    using base_t = edge_list_edges<Dir, V, EC, two_vertices>;
    using base_t::edges_;

public:
    using vertex = typename base_t::vertex;
    using edge   = typename base_t::edge;

    using base_t::base_t;

    class edge_iterator
        : public iterator_facade<edge_iterator, std::input_iterator_tag, edge, edge> {
        friend struct ac::iterator_core_access;
        friend class edge_list_impl;

        using ec_id = container_id<typename base_t::edge_container>;

        edge_iterator(const edge_list_impl* el, ec_id id) : el_(el), id_(id) {}

        edge dereference() const {
            auto& vertices = el_->edges_[id_].first();
            return edge(vertices.u, vertices.v, id_);
        }

        void increment() { ++id_; }

        bool equals(const edge_iterator& other) const { return id_ == other.id_; }

        const edge_list_impl* el_;
        ec_id id_;
    };

    range<edge_iterator> edges() const {
        return {edge_iterator(this, begin_id(edges_)), edge_iterator(this, end_id(edges_))};
    }

    edge find_edge(vertex u, vertex v) const {
        if constexpr (base_t::is_undirected) {
            if (get_id_key(u) > get_id_key(u)) std::swap(u, v);
        }
        return edge(u, v, find(edges_, typename base_t::edge_vertices(u, v)));
    }

    void remove_edge(edge e) { erase(edges_, e); }

    void swap(edge_list_impl& other) { base_t::swap(other); }
};

}  // namespace detail

template <class Directed, class Vertex, class EdgeContainer = std::vector<none>>
using edge_list = detail::edge_list_impl<Directed, Vertex, EdgeContainer, two_vertices>;

}  // namespace ac