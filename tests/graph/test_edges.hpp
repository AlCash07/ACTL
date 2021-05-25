// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/graph/traits.hpp>
#include <actl/map/traits.hpp>
#include <actl/std/map.hpp>
#include "graph/bundle.hpp"
#include "graph/edges_ends.hpp"
#include "test.hpp"

template <bool TestAdjacency = false, class Graph, class V = vertex_t<Graph>>
void test_edges(Graph& graph, V v0, V v1, V v2)
{
    std::vector<edge_t<Graph>> es;
    es.push_back(graph.add_edge(v0, v1, 0, "e01"));
    es.push_back(graph.add_edge(v0, v2, 1, "e02"));
    es.push_back(graph.add_edge(v2, v1, 2, "e21"));
    es.push_back(graph.add_edge(v2, v2, 8, "e22"));
    auto [e01, ok01] = graph.try_add_edge(v0, v1, 3, "e01a");
    CHECK(Graph::allows_parallel_edges == ok01);
    if (ok01)
        es.push_back(e01);
    auto [e10, ok10] = graph.try_add_edge(v1, v0, 4, "e10");
    CHECK((Graph::allows_parallel_edges || Graph::is_directed == ok10));
    if (ok10)
        es.push_back(e10);
    auto map = graph[edge_property{}];
    graph[es[2]].s = "e21a";
    put(map, es[0], bundle(0, "e01b"));
    CHECK("e01b"sv == graph[es[0]].s);
    CHECK("e02"sv == graph[es[1]].s);
    CHECK("e21a"sv == get(map, es[2]).s);
    auto e_range = graph.edges();
    CHECK_EQUAL_SETS(
        get_ends<Graph::is_directed>(es),
        get_ends<Graph::is_directed>(e_range));
    auto e02 = graph.find_edge(v0, v2);
    CHECK_EQUAL_SETS(std::vector{v0, v2}, {e02.source(), e02.target()});
    CHECK(es[1] == e02);
    if constexpr (TestAdjacency)
    {
        std::map<V, std::vector<V>> outs, ins;
        auto add_edge = [&](V u, V v)
        {
            outs[u].push_back(v);
            if constexpr (Graph::is_undirected)
                if (u != v)
                    outs[v].push_back(u);
            if constexpr (Graph::is_bidirectional)
                ins[v].push_back(u);
        };
        add_edge(v0, v1);
        add_edge(v0, v2);
        add_edge(v2, v1);
        add_edge(v2, v2);
        if (ok01)
            add_edge(v0, v1);
        if (ok10)
            add_edge(v1, v0);
        for (auto v : {v0, v1, v2})
        {
            CHECK_EQUAL_SETS(outs[v], get_targets(graph.out_edges(v)));
            if constexpr (Graph::is_undirected)
                CHECK_EQUAL_SETS(outs[v], get_sources(graph.in_edges(v)));
            if constexpr (Graph::is_bidirectional)
                CHECK_EQUAL_SETS(ins[v], get_sources(graph.in_edges(v)));
        }
    };
    // TODO: uncomment when remove_edge is implemented in adjacency_list.
    // graph.remove_edge(e02);
    // CHECK(3 + ok01 + ok10 == graph.edge_count());
}
