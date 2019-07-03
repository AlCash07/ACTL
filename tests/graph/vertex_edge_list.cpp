/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/graph/vertex_edge_list.hpp>
#include <actl/std/all.hpp>
#include <actl/test.hpp>

TEST("vector") {
    vertex_edge_list<directed, std::vector<std::string>, std::vector<std::string>> graph(2);
    auto e = graph.add_edge(0, 2, "e02");
    ASSERT_EQUAL(3, graph.vertex_count());
    ASSERT_EQUAL(1, graph.edge_count());
    graph[e.target()] = "v2";
    ASSERT_EQUAL("", graph[1]);
    ASSERT_EQUAL("v2", graph[2]);
    ASSERT_EQUAL("e02", graph[e]);
    graph = {};
    ASSERT_EQUAL(0, graph.vertex_count());
    ASSERT_EQUAL(0, graph.edge_count());
}

TEST("set") {
    vertex_edge_list<directed, std::vector<std::string>, std::set<std::string>> graph;
    auto e = graph.add_edge("v0", "v2", "e02");
    ASSERT_EQUAL(2, graph.vertex_count());
    ASSERT_EQUAL(1, graph.edge_count());
    auto v = e.target();
    graph[v] = "v2a";
    ASSERT_EQUAL("v0", graph[e.source()]);
    ASSERT_EQUAL("v2a", graph[v]);
    ASSERT_EQUAL("e02", graph[e]);
    graph = {};
    ASSERT_EQUAL(0, graph.vertex_count());
    ASSERT_EQUAL(0, graph.edge_count());
}
