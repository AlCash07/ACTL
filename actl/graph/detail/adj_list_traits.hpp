/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/edge_list_traits.hpp>
#include <actl/graph/edge_list.hpp>
#include <actl/graph/vertex_list.hpp>

namespace ac::detail {

template <class Dir, class OEC>
struct vertex_edges {
    OEC out_edges;
};

template <class OEC>
struct vertex_edges<bidirectional, OEC> : vertex_edges<directed, OEC> {
    OEC in_edges;
};

template <class Dir, class OEC, class EC, class VC>
class adj_list_vertex_data;

template <class Dir, class OEC, class EC, class VC>
struct adj_list_traits {
    using edge_selector = value_type_t<EC>;

    using vertices = vertex_list<
        typename container_traits<VC>::template rebind<adj_list_vertex_data<Dir, OEC, EC, VC>>>;

    using vertex_id = typename vertices::vertex_id;

    using edges = edge_list_base<Dir, vertex_id,
                                 typename container_traits<EC>::template rebind<value_type_t<OEC>>,
                                 edge_selector, std::is_same_v<edge_selector, two_vertices>>;

    // Out edge must contain target vertex as key in associative container.
    using out_edge_selector =
        std::conditional_t<is_associative_v<OEC> && (std::is_same_v<edge_selector, one_vertex> ||
                                                     std::is_same_v<edge_selector, two_vertices>),
                           edge_property, edge_selector>;

    using out_edge_vertex = std::conditional_t<std::is_same_v<out_edge_selector, edge_property>,
                                               wrap_id<vertex_id>, none>;

    using edge_bundle = value_type_t<OEC>;

    using out_edge_bundle = std::conditional_t<
        !std::is_same_v<edge_selector, none>, typename edges::edge_id,
        // Edge bundle is duplicated if graph is undirected or bidirectional, so it's immutable.
        std::conditional_t<std::is_same_v<Dir, directed>, edge_bundle, const edge_bundle>>;

    using out_edge_data = bundle_decorator<out_edge_vertex, out_edge_bundle>;

    using out_edge_container =
        generic_container<typename container_traits<OEC>::template rebind<out_edge_data>>;

    using vertex_edges = vertex_edges<Dir, out_edge_container>;
    using vertex_data  = bundle_decorator<vertex_edges, value_type_t<VC>, false>;
};

template <class Dir, class OEC, class EC, class VC>
class adj_list_vertex_data : public adj_list_traits<Dir, OEC, EC, VC>::vertex_data {
public:
    template <class... Ts>
    explicit adj_list_vertex_data(Ts&&... args)
        : adj_list_traits<Dir, OEC, EC, VC>::vertex_data(
              typename adj_list_traits<Dir, OEC, EC, VC>::vertex_edges(),
              std::forward<Ts>(args)...) {}
};

template <class S, class VId, class EId>
struct adj_list_edge_id {};

template <class VId, class EId>
struct adj_list_edge_id<two_vertices, VId, EId> {
    using type = EId;
};

template <class VId, class EId>
struct adj_list_edge_id<one_vertex, VId, EId> {
    struct type {
        VId u;
        EId e;

        constexpr operator EId() const { return e; }
    };
};

template <class VId, class EId>
struct adj_list_edge_id<edge_property, VId, EId> {
    struct type {
        VId u;
        VId v;
        EId e;

        constexpr operator EId() const { return e; }
    };
};

template <class VId, class OEId, class S>
struct full_edge_id {
    VId  u;
    OEId e_out;
    OEId e_in;
};

template <class VId, class OEId>
struct full_edge_id<VId, OEId, directed> {
    VId  u;
    OEId e_out;
};

}  // namespace ac::detail

namespace std {

template <class Dir, class OEC, class EC, class VC>
struct hash<ac::detail::adj_list_vertex_data<Dir, OEC, EC, VC>>
    : hash<typename ac::detail::adj_list_traits<Dir, OEC, EC, VC>::vertex_data> {};

}
