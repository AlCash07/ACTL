/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/associative_property_map.hpp>
#include <actl/property_map/container_property_map.hpp>
#include <actl/traits/container_traits.hpp>
#include <map>
#include <vector>

namespace ac {

template <class T, class Graph>
inline auto default_vertex_property_map(const Graph& graph) {
    if constexpr (is_random_access_container_v<typename Graph::vertex_container>) {
        return make_container_property_map(std::vector<T>(graph.vertex_count()));
    } else {
        return make_associative_property_map(std::map<typename Graph::vertex, T>());
    }
}

template <class T, class Graph>
inline auto default_edge_property_map(const Graph& graph) {
    if constexpr (is_random_access_container_v<typename Graph::edge_container>) {
        return make_container_property_map(std::vector<T>(graph.edge_count()));
    } else {
        return make_associative_property_map(std::map<typename Graph::edge, T>());
    }
}

}  // namespace ac
