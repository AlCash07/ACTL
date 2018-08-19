/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/edge.hpp>
#include <actl/graph/detail/edge_list_traits.hpp>
#include <actl/graph/detail/none_property_map.hpp>
#include <actl/property_map/generic_container_property_map.hpp>

namespace ac {

template <class Directed, class VertexId, class EdgeContainer = std::vector<none>>
class edge_list;

namespace detail {

template <class Dir, class V, class EC, class S, class T = value_type_t<EC>>
class edge_list_base : public edge_list_base<Dir, V, EC, S, none> {
    using base_t = edge_list_base<Dir, V, EC, S, none>;

public:
    using edge = typename base_t::edge;

    using base_t::base_t;

    auto operator[](edge_property) {
        return get_second(make_generic_container_property_map(this->edges_));
    }

    auto operator[](edge_property) const {
        return get_second(make_generic_container_property_map(this->edges_));
    }

    T&       operator[](edge e)       { return get((*this)[edge_property{}], e); }
    const T& operator[](edge e) const { return get((*this)[edge_property{}], e); }
};

template <class Dir, class V, class EC, class S>
class edge_list_base<Dir, V, EC, S, none> {
protected:
    using traits         = edge_list_traits<Dir, V, EC, S>;
    using edge_vertices  = typename traits::vertices;
    using edge_container = typename traits::container;

    edge_container edges_;

public:
    static_assert(std::is_same_v<S, two_vertices> || !is_associative_v<EC>,
                  "associative edge list requires two vertices");

    using vertex = V;
    using edge   = detail::edge<V, typename edge_container::id>;

    using directed_category = Dir;

    static constexpr bool is_undirected         = std::is_same_v<Dir, undirected>;
    static constexpr bool is_directed           = !is_undirected;
    static constexpr bool allows_parallel_edges = !is_unique_associative<EC>::value;

    explicit edge_list_base() = default;

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

    void clear() { edges_.clear(); }

    void swap(edge_list_base& other) { edges_.swap(other.edges_); }

    detail::none_property_map<edge> operator[](edge_property) const { return {}; }

    none operator[](edge)       { return none{}; }
    none operator[](edge) const { return none{}; }
};

}  // namespace detail

template <class Dir, class V, class EC>
class edge_list : public detail::edge_list_base<Dir, V, EC, two_vertices> {
    using base_t = detail::edge_list_base<Dir, V, EC, two_vertices>;
    using ec_id  = typename base_t::edge_container::id;
    using base_t::edges_;

public:
    using vertex = typename base_t::vertex;
    using edge   = typename base_t::edge;

    using base_t::base_t;

    class edge_iterator
        : public iterator_facade<edge_iterator, std::input_iterator_tag, edge, edge> {
        friend struct iterator_core_access;
        friend class edge_list;

        edge dereference() const { return el_->get_edge(id_); }

        void increment() { ++id_; }

        bool equals(const edge_iterator& other) const { return id_ == other.id_; }

        ec_id id_;
        const edge_list* el_;
    };

    range<edge_iterator> edges() const {
        return {edge_iterator{edges_.begin_id(), this}, edge_iterator{edges_.end_id(), this}};
    }

    edge null_edge() const { return edges_.null_id(); }

    edge find_edge(vertex u, vertex v) const {
        if constexpr (base_t::is_undirected) {
            if (get_id_key(u) > get_id_key(u)) std::swap(u, v);
        }
        return {u, v, this->edges_.find(typename base_t::edge_vertices(u, v))};
    }

    void remove_edge(edge e) { edges_.erase(e); }

    void swap(edge_list& other) { base_t::swap(other); }

private:
    edge get_edge(ec_id id) const { return {edges_[id].u, edges_[id].v, id}; }
};

}  // namespace ac
