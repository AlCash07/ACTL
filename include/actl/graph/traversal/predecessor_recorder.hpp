// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/vertex_initializer.hpp>

namespace ac {

template <class Map>
struct predecessor_recorder
{
    void operator()(on_search_start, map_key_t<Map> u)
    {
        put(map, u, u);
    }

    template <class E>
    void operator()(on_tree_edge_start, E e)
    {
        put(map, e.target(), e.source());
    }

    template <class E>
    void operator()(on_edge_relaxed, E e)
    {
        operator()(on_tree_edge_start{}, e);
    }

    Map map;
};

template <class Map>
predecessor_recorder(Map&&) -> predecessor_recorder<Map>;

template <class Map, class T>
vertex_initializer<predecessor_recorder<Map>> make_predecessor_recorder(
    Map&& predecessor, T value)
{
    return {{std::forward<Map>(predecessor)}, value};
}

template <class Graph>
auto make_default_predecessor_recorder(Graph const& graph)
{
    return make_predecessor_recorder(
        make_default_vertex_map<vertex_t<Graph>>(graph), graph.null_vertex());
}

} // namespace ac
