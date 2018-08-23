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
    using edge_selector = value_type_t<EC>;

    using vertex_list = vertex_list<
        typename container_traits<VC>::template rebind<adj_list_vertex_data<Dir, OEC, EC, VC>>>;

    using vertex = typename vertex_list::vertex;

    using edge_list =
        edge_list_impl<Dir, vertex,
                       typename container_traits<EC>::template rebind<value_type_t<OEC>>,
                       edge_selector>;

    // Out edge must contain target vertex as key in associative container.
    using out_edge_vertex =
        std::conditional_t<is_associative_v<OEC> || std::is_same_v<edge_selector, edge_property> ||
                               std::is_same_v<edge_selector, none>,
                           vertex, none>;

    using out_edge_bundle = std::conditional_t<!std::is_same_v<edge_selector, none>,
                                               typename edge_list::edge_id, value_type_t<OEC>>;

    using out_edge_data = mimic_pair<out_edge_vertex, out_edge_bundle, 1>;

    using out_edge_container =
        generic_container<typename container_traits<OEC>::template rebind<out_edge_data>>;

    // In bidirectional graph with none edge_selector in_edge points to out_edge.
    using in_edge_bundle =
        std::conditional_t<std::is_same_v<edge_selector, none> &&
                               std::is_same_v<Dir, bidirectional>,
                           typename out_edge_container::id, typename edge_list::edge_id>;

    using in_edge_data = mimic_pair<out_edge_vertex, in_edge_bundle, 1>;

    using in_edge_container =
        generic_container<typename container_traits<OEC>::template rebind<in_edge_data>>;

    using vertex_edges = vertex_edges<Dir, out_edge_container, in_edge_container>;
    using vertex_data  = mimic_pair<vertex_edges, value_type_t<VC>, 2>;
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

namespace std {

template <class Dir, class OEC, class EC, class VC>
struct hash<ac::detail::adj_list_vertex_data<Dir, OEC, EC, VC>>
    : hash<typename ac::detail::adj_list_traits<Dir, OEC, EC, VC>::vertex_data> {};

}  // namespace std
