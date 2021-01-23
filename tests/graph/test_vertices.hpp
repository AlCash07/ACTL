// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/selectors.hpp>
#include <actl/graph/traits.hpp>
#include <actl/map/traits.hpp>
#include "graph/bundle.hpp"
#include "test.hpp"

template <class Graph, class V = vertex_t<Graph>>
std::vector<V> test_vertices(Graph& graph) {
    std::vector<V> vs;
    vs.push_back(graph.add_vertex(0, "v0"));
    vs.push_back(graph.add_vertex(1, "v1"));
    vs.push_back(graph.add_vertex(2, "v2"));
    CHECK(3l == graph.vertex_count());
    auto [v3, ok] = graph.try_add_vertex(1, "v3");
    CHECK_FALSE(is_unique_range_v<typename Graph::vertex_container> == ok);
    if (ok)
        vs.push_back(v3);
    auto map = graph[vertex_property{}];
    graph[vs[2]].s = "v2a";
    put(map, vs[0], bundle(0, "v0a"));
    CHECK("v0a"sv == graph[vs[0]].s);
    CHECK("v1"sv == graph[vs[1]].s);
    CHECK("v2a"sv == get(map, vs[2]).s);
    auto v_range = graph.vertices();
    CHECK_EQUAL_SETS(vs, std::vector<V>(v_range.begin(), v_range.end()));
    return vs;
}
