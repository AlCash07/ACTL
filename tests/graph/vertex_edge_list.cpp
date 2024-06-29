// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/graph/vertex_edge_list.hpp>
#include <actl/std/all.hpp>
#include "test.hpp"

TEST_CASE("vertex_edge_list vector") {
    vertex_edge_list<
        directed,
        std::vector<std::string>,
        std::vector<std::string>>
        graph(2);
    auto e = graph.add_edge(0, 2, "e02");
    CHECK(3l == graph.vertex_count());
    CHECK(1l == graph.edge_count());
    graph[e.target()] = "v2";
    CHECK(""sv == graph[1]);
    CHECK("v2"sv == graph[2]);
    CHECK("e02"sv == graph[e]);
    graph = {};
    CHECK(0l == graph.vertex_count());
    CHECK(0l == graph.edge_count());
}

TEST_CASE("vertex_edge_list set") {
    vertex_edge_list<directed, std::vector<std::string>, std::set<std::string>>
        graph;
    auto e = graph.add_edge("v0", "v2", "e02");
    CHECK(2l == graph.vertex_count());
    CHECK(1l == graph.edge_count());
    auto v = e.target();
    graph[v] = "v2a";
    CHECK("v0"sv == graph[e.source()]);
    CHECK("v2a"sv == graph[v]);
    CHECK("e02"sv == graph[e]);
    graph = {};
    CHECK(0l == graph.vertex_count());
    CHECK(0l == graph.edge_count());
}
