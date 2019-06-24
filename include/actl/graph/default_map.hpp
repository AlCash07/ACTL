/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/associative_map.hpp>
#include <actl/map/container_map.hpp>
#include <actl/std/map.hpp>
#include <actl/std/vector.hpp>
#include <actl/traits/container_traits.hpp>

namespace ac {

template <class T, class Graph>
inline auto make_default_vertex_map(const Graph& graph) {
    if constexpr (is_random_access_container_v<typename Graph::vertex_container>) {
        return container_map{std::vector<T>(static_cast<size_t>(graph.vertex_count()))};
    } else {
        return associative_map{std::map<typename Graph::vertex, T>{}};
    }
}

template <class T, class Graph>
inline auto make_default_edge_map(const Graph& graph) {
    if constexpr (is_random_access_container_v<typename Graph::edge_container>) {
        return container_map{std::vector<T>(static_cast<size_t>(graph.edge_count()))};
    } else {
        return associative_map{std::map<typename Graph::edge, T>{}};
    }
}

}  // namespace ac
