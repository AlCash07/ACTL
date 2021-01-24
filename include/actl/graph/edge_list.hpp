// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/detail/edge.hpp>
#include <actl/graph/detail/edge_list_traits.hpp>
#include <actl/map/generic_container.hpp>
#include <actl/std/vector.hpp>

namespace ac {

namespace detail {

template <class Dir, class V, class EC, class S, class T = value_type_t<EC>>
class edge_list_edges : public edge_list_edges<Dir, V, EC, S, none> {
    using base_t = edge_list_edges<Dir, V, EC, S, none>;

public:
    using typename base_t::edge;

    using base_t::base_t;

    auto operator[](edge_property) {
        return get_second(this->edges_);
    }
    auto operator[](edge_property) const {
        return get_second(this->edges_);
    }

    T& operator[](edge e) {
        return get((*this)[edge_property{}], e);
    }
    const T& operator[](edge e) const {
        return get((*this)[edge_property{}], e);
    }
};

template <class Dir, class V, class EC, class S>
class edge_list_edges<Dir, V, EC, S, none> {
protected:
    using traits = edge_list_traits<Dir, V, EC, S>;
    using edge_vertices = typename traits::vertices;

public:
    static_assert(
        std::is_same_v<S, two_vertices> || !is_associative_range_v<EC>,
        "associative edge list requires two vertices");

    using edge_container = typename traits::container;
    using edge_id = container_id<edge_container>;
    using vertex = V;
    using edge = edge<V, edge_id>;

    using directed_category = Dir;

    static constexpr bool is_undirected = std::is_same_v<Dir, undirected>;
    static constexpr bool is_directed = !is_undirected;
    static constexpr bool allows_parallel_edges = !is_unique_range_v<EC>;

    edge_list_edges() = default;

    index edge_count() const {
        return static_cast<index>(edges_.size());
    }

    template <class... Ts>
    std::pair<edge, bool> try_add_edge(vertex u, vertex v, Ts&&... args) {
        if constexpr (is_undirected) {
            if (v < u)
                std::swap(u, v);
        }
        auto res = id_emplace(edges_, edge_vertices(u, v), std::forward<Ts>(args)...);
        return {edge{u, v, res.first}, res.second};
    }

    template <class... Ts>
    edge add_edge(vertex u, vertex v, Ts&&... args) {
        return try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    vertex get_target(vertex u, edge_id e) const {
        return id_at(edges_, e).first().other(u);
    }

    void swap(edge_list_edges& rhs) {
        edges_.swap(rhs.edges_);
    }

    void operator[](edge) const {}

protected:
    edge_container edges_;
};

template <class Dir, class V, class EC, class S>
class edge_list_impl : public edge_list_edges<Dir, V, EC, S> {
    using base_t = edge_list_edges<Dir, V, EC, S>;

public:
    using base_t::base_t;

    void swap(edge_list_impl& rhs) {
        base_t::swap(rhs);
    }
};

template <class Dir, class V, class EC>
class edge_list_impl<Dir, V, EC, two_vertices> : public edge_list_edges<Dir, V, EC, two_vertices> {
    using base_t = edge_list_edges<Dir, V, EC, two_vertices>;
    using base_t::edges_;

    template <class E>
    struct edge_iterator_types {
        using iterator_category = std::input_iterator_tag;
        using value_type = E;
        using reference = E;
        using difference_type = use_default;
    };

public:
    using typename base_t::edge;
    using typename base_t::vertex;

    using base_t::base_t;

    template <class E>
    class edge_iterator : public iterator_facade<edge_iterator<E>, edge_iterator_types<E>> {
        friend struct ac::iterator_core_access;
        friend class edge_list_impl;

        using ec_id = container_id<typename base_t::edge_container>;

        edge_iterator(const edge_list_impl& el, ec_id id) : el_{el}, id_{id} {}

        E dereference() const {
            auto& vertices = id_at(el_.edges_, id_).first();
            using V1 = typename E::vertex;
            return E{V1{vertices.u}, V1{vertices.v}, id_};
        }

        void increment() {
            ++id_;
        }

        bool equals(const edge_iterator& rhs) const {
            return id_ == rhs.id_;
        }

        const edge_list_impl& el_;
        ec_id id_;
    };

    // E template parameter is needed for adjacency_list where vertices aren't stored directly.
    template <class E = edge>
    iterator_range<edge_iterator<E>> edges() const {
        return {edge_iterator<E>{*this, id_begin(edges_)}, edge_iterator<E>{*this, id_end(edges_)}};
    }

    edge find_edge(vertex u, vertex v) const {
        if constexpr (base_t::is_undirected) {
            if (v < u)
                std::swap(u, v);
        }
        return edge{u, v, id_find(edges_, typename base_t::edge_vertices(u, v))};
    }

    void remove_edge(edge e) {
        id_erase(edges_, e);
    }

    void swap(edge_list_impl& rhs) {
        base_t::swap(rhs);
    }
};

}  // namespace detail

template <class Directed, class Vertex, class EdgeContainer = std::vector<none>>
using edge_list = detail::edge_list_impl<Directed, Vertex, EdgeContainer, two_vertices>;

}  // namespace ac
