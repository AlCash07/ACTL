// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/traits.hpp>
#include <actl/map/generic_container.hpp>
#include <actl/map/pair_associative_container.hpp>
#include <actl/std/map.hpp>
#include <actl/std/vector.hpp>

namespace ac {

template <class T, class Graph>
auto make_default_vertex_map(const Graph& graph) {
    if constexpr (is_random_access_range_v<typename Graph::vertex_container>) {
        return std::vector<T>(static_cast<size_t>(graph.vertex_count()));
    } else {
        return std::map<vertex_t<Graph>, T>{};
    }
}

template <class T, class Graph>
auto make_default_edge_map(const Graph& graph) {
    if constexpr (is_random_access_range_v<typename Graph::edge_container>) {
        return std::vector<T>(static_cast<size_t>(graph.edge_count()));
    } else {
        return std::map<edge_t<Graph>, T>{};
    }
}

} // namespace ac
