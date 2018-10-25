/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/selectors.hpp>
#include <actl/property_map/property_map.hpp>
#include <actl/test.hpp>
#include "bundle.hpp"

template <class Graph, class V = typename Graph::vertex>
inline std::vector<V> test_vertices(Graph& graph) {
    using namespace ac;
    std::vector<V> vs;
    vs.push_back(graph.add_vertex(0, "v0"));
    vs.push_back(graph.add_vertex(1, "v1"));
    vs.push_back(graph.add_vertex(2, "v2"));
    ASSERT_EQUAL(3, graph.vertex_count());
    auto[v3, ok] = graph.try_add_vertex(1, "v3");
    ASSERT_EQUAL(!is_unique_associative_container_v<typename Graph::vertex_container>, ok);
    if (ok) vs.push_back(v3);
    auto pm = graph[vertex_property{}];
    graph[vs[2]].s = "v2a";
    put(pm, vs[0], bundle(0, "v0a"));
    ASSERT_EQUAL("v0a", graph[vs[0]].s);
    ASSERT_EQUAL("v1", graph[vs[1]].s);
    ASSERT_EQUAL("v2a", get(pm, vs[2]).s);
    auto v_range = graph.vertices();
    ASSERT_EQUAL_SETS(vs, {v_range.begin(), v_range.end()});
    return vs;
}
