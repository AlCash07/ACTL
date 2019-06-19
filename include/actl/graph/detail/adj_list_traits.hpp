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

template <class Dir, class OEC, class IEC>
struct vertex_edges {
    OEC out_edges;
};

template <class OEC, class IEC>
struct vertex_edges<bidirectional, OEC, IEC> : vertex_edges<directed, OEC, IEC> {
    IEC in_edges;
};

template <class Dir, class OEC, class EC, class VC>
class adj_list_vertex_data;

template <class Dir, class OEC, class EC, class VC>
struct adj_list_traits {
    using vertex = std::conditional_t<is_random_access_container_v<VC>, int, void*>;

    using edge_selector = value_type_t<EC>;

    using edges =
        edge_list_impl<Dir, vertex, rebind_container_t<EC, value_type_t<OEC>>, edge_selector>;

    // Out edge must contain target vertex as key in associative container.
    static constexpr bool has_out_vertex = is_associative_container_v<OEC> ||
                                           std::is_same_v<edge_selector, edge_property> ||
                                           std::is_same_v<edge_selector, none>;

    using out_edge_vertex = std::conditional_t<has_out_vertex, vertex, none>;

    using out_edge_bundle = std::conditional_t<!std::is_same_v<edge_selector, none>,
                                               typename edges::edge_id, value_type_t<OEC>>;

    using out_edge_data = mimic_pair<out_edge_vertex, out_edge_bundle, 1>;

    using out_edge_container = rebind_container_t<OEC, out_edge_data>;

    // In bidirectional graph with none edge_selector in_edge points to out_edge.
    using in_edge_bundle =
        std::conditional_t<std::is_same_v<edge_selector, none> &&
                               std::is_same_v<Dir, bidirectional>,
                           container_id<out_edge_container>, typename edges::edge_id>;

    using in_edge_data = mimic_pair<out_edge_vertex, in_edge_bundle, 1>;

    using in_edge_container = rebind_container_t<OEC, in_edge_data>;

    using vertex_edges = vertex_edges<Dir, out_edge_container, in_edge_container>;
    using vertex_data  = mimic_pair<vertex_edges, value_type_t<VC>, 2>;

    using vertices = vertex_list<rebind_container_t<VC, adj_list_vertex_data<Dir, OEC, EC, VC>>>;

    // vertices::vertex cannot be used as an element inside vertices because it would result in
    // incomplete type inside stl container which is undefined behaviour.
    // One solution would be to store iterators on heap and maintain pointers to them, however this
    // is ugly and adds heap allocation and dereferencing overhead.
    // Here we operate based on assumption that iterators for supported containers are just wrapped
    // pointers (and it's true for all the major stl implementations), so they can be reinterpreted
    // as void*.
    static_assert(std::is_same_v<vertex, int> ||
                  sizeof(typename vertices::vertex) == sizeof(void*));
};

template <class Dir, class OEC, class EC, class VC>
class adj_list_vertex_data : public adj_list_traits<Dir, OEC, EC, VC>::vertex_data {
    using traits = adj_list_traits<Dir, OEC, EC, VC>;

public:
    template <class... Ts>
    explicit adj_list_vertex_data(Ts&&... args)
        : traits::vertex_data{typename traits::vertex_edges{}, std::forward<Ts>(args)...} {}
};

template <class V, class OE, class S>
struct full_edge {
    V  u;
    OE e_out;
    OE e_in;
};

template <class V, class OE>
struct full_edge<V, OE, directed> {
    V  u;
    OE e_out;
};

}  // namespace ac::detail
