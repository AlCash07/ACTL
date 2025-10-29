// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/detail/edge.hpp>
#include <actl/graph/detail/edge_list_traits.hpp>
#include <actl/map/generic_container.hpp>
#include <actl/range/iterator/interface/forward_iterator_interface.hpp>
#include <actl/std/vector.hpp>

namespace ac {

namespace detail {

template<
    class Dir,
    class V,
    class EC,
    class S,
    class T = graph::list_value_t<EC>>
class edge_list_edges : public edge_list_edges<Dir, V, EC, S, none> {
    using base_t = edge_list_edges<Dir, V, EC, S, none>;

public:
    using typename base_t::edge;

    using base_t::base_t;

    auto operator[](edge_property) {
        return get_second(this->m_edges);
    }

    auto operator[](edge_property) const {
        return get_second(this->m_edges);
    }

    T& operator[](edge e) {
        return get((*this)[edge_property{}], e);
    }

    T const& operator[](edge e) const {
        return get((*this)[edge_property{}], e);
    }
};

template<class Dir, class V, class EC, class S>
class edge_list_edges<Dir, V, EC, S, none> {
protected:
    using traits = edge_list_traits<Dir, V, EC, S>;
    using edge_vertices = typename traits::vertices;

public:
    static_assert(
        std::is_same_v<S, two_vertices> || !AssociativeRange<EC>,
        "associative edge list requires two vertices"
    );

    using edge_container = typename traits::container;
    using edge_id = container_id<edge_container>;
    using vertex = V;
    using edge = edge<V, edge_id>;

    using directed_category = Dir;

    static constexpr bool is_undirected = std::is_same_v<Dir, undirected>;
    static constexpr bool is_directed = !is_undirected;
    static constexpr bool allows_parallel_edges = !UniqueRange<EC>;

    edge_list_edges() = default;

    index edge_count() const {
        return static_cast<index>(m_edges.size());
    }

    template<class... Ts>
    std::pair<edge, bool> try_add_edge(vertex u, vertex v, Ts&&... args) {
        if constexpr (is_undirected) {
            if (v < u)
                std::swap(u, v);
        }
        auto res =
            id_emplace(m_edges, edge_vertices(u, v), std::forward<Ts>(args)...);
        return {edge{u, v, res.first}, res.second};
    }

    template<class... Ts>
    edge add_edge(vertex u, vertex v, Ts&&... args) {
        return try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    vertex get_target(vertex u, edge_id e) const {
        return id_at(m_edges, e).first.other(u);
    }

    void swap(edge_list_edges& rhs) {
        m_edges.swap(rhs.m_edges);
    }

    void operator[](edge) const {}

protected:
    edge_container m_edges;
};

template<class Dir, class V, class EC, class S>
class edge_list_impl : public edge_list_edges<Dir, V, EC, S> {
    using base_t = edge_list_edges<Dir, V, EC, S>;

public:
    using base_t::base_t;

    void swap(edge_list_impl& rhs) {
        base_t::swap(rhs);
    }
};

template<class Dir, class V, class EC>
class edge_list_impl<Dir, V, EC, two_vertices>
    : public edge_list_edges<Dir, V, EC, two_vertices> {
    using base_t = edge_list_edges<Dir, V, EC, two_vertices>;
    using base_t::m_edges;

public:
    using typename base_t::edge;
    using typename base_t::vertex;

    using base_t::base_t;

    template<class E>
    class edge_iterator : public forward_iterator_interface<edge_iterator<E>> {
    public:
        using value_type = E;
        using difference_type = std::ptrdiff_t;

        edge_iterator() = default;

        E operator*() const {
            auto& vertices = id_at(m_el->m_edges, m_id).first;
            using V1 = typename E::vertex;
            return E{V1{vertices.u}, V1{vertices.v}, m_id};
        }

        edge_iterator& operator++() {
            ++m_id;
            return *this;
        }

        friend bool operator==(
            edge_iterator const& lhs, edge_iterator const& rhs
        ) AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs.m_id == rhs.m_id)

    private:
        friend class edge_list_impl;

        using ec_id = container_id<typename base_t::edge_container>;

        edge_iterator(edge_list_impl const& el, ec_id id)
            : m_el{&el}, m_id{id} {}

        edge_list_impl const* m_el;
        ec_id m_id;
    };

    // E template parameter is needed for adjacency_list where vertices aren't
    // stored directly.
    template<class E = edge>
    iterator_range<edge_iterator<E>> edges() const {
        return {
            edge_iterator<E>{*this, id_begin(m_edges)},
            edge_iterator<E>{*this, id_end(m_edges)}
        };
    }

    edge find_edge(vertex u, vertex v) const {
        if constexpr (base_t::is_undirected) {
            if (v < u)
                std::swap(u, v);
        }
        return edge{
            u, v, id_find(m_edges, typename base_t::edge_vertices(u, v))
        };
    }

    void remove_edge(edge e) {
        id_erase(m_edges, e);
    }

    void swap(edge_list_impl& rhs) {
        base_t::swap(rhs);
    }
};

} // namespace detail

template<class Directed, class Vertex, class EdgeContainer = std::vector<none>>
using edge_list =
    detail::edge_list_impl<Directed, Vertex, EdgeContainer, two_vertices>;

} // namespace ac
