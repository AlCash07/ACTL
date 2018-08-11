/***************************************************************************************************
 * Class that adds vertices container to adjacency list.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/adj_list_traits.hpp>

namespace ac::detail {

template <class Dir, class OEC, class EC, class VC, class T = value_type_t<VC>>
class adj_list_vertices : public adj_list_vertices<Dir, OEC, EC, VC, none> {
    using base_t = adj_list_vertices<Dir, OEC, EC, VC, none>;

public:
    using vertex_id = typename base_t::vertex_id;

    using base_t::base_t;

    auto operator[](vertex_property) {
        return append_bundle_property_map(make_generic_container_property_map(this->vertices_));
    }

    auto operator[](vertex_property) const {
        return append_bundle_property_map(make_generic_container_property_map(this->vertices_));
    }

    T&       operator[](vertex_id v)       { return get((*this)[vertex_property{}], v); }
    const T& operator[](vertex_id v) const { return get((*this)[vertex_property{}], v); }
};

template <class Dir, class OEC, class EC, class VC>
class adj_list_vertices<Dir, OEC, EC, VC, none>
    : public adj_list_traits<Dir, OEC, EC, VC>::vertices {
    using base_t = typename adj_list_traits<Dir, OEC, EC, VC>::vertices;

public:
    using directed_category = Dir;

    static constexpr bool is_undirected         = std::is_same_v<Dir, undirected>;
    static constexpr bool is_directed           = !is_undirected;
    static constexpr bool is_bidirectional      = std::is_same_v<Dir, bidirectional>;
    static constexpr bool allows_parallel_edges = !is_unique_associative<OEC>::value;

    using base_t::base_t;
};

}  // namespace ac::detail