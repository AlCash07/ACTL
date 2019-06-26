/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/map/map.hpp>
#include <actl/test.hpp>
#include "bundle.hpp"

template <bool Directed, class E>
inline auto get_ends(const std::vector<E>& es) {
    using namespace ac;
    using V = typename E::vertex;
    std::vector<std::pair<V, V>> res;
    for (auto e : es) {
        V u = e.source();
        V v = e.target();
        if constexpr (!Directed) {
            if (v < u) std::swap(u, v);
        }
        res.emplace_back(u, v);
    }
    return res;
}

template <class R>
inline auto get_sources(const R& es) {
    std::vector<typename R::value_type::vertex> res;
    for (auto e : es) res.emplace_back(e.source());
    return res;
}

template <class R>
inline auto get_targets(const R& es) {
    std::vector<typename R::value_type::vertex> res;
    for (auto e : es) res.emplace_back(e.target());
    return res;
}

template <bool TestAdjacency = false, class Graph, class V = typename Graph::vertex>
inline void test_edges(Graph& graph, V v0, V v1, V v2) {
    using namespace ac;
    using E = typename Graph::edge;
    std::vector<E> es;
    es.push_back(graph.add_edge(v0, v1, 0, "e01"));
    es.push_back(graph.add_edge(v0, v2, 1, "e02"));
    es.push_back(graph.add_edge(v2, v1, 2, "e21"));
    es.push_back(graph.add_edge(v2, v2, 8, "e22"));
    auto[e01, ok01] = graph.try_add_edge(v0, v1, 3, "e01a");
    ASSERT_EQUAL(Graph::allows_parallel_edges, ok01);
    if (ok01) es.push_back(e01);
    auto[e10, ok10] = graph.try_add_edge(v1, v0, 4, "e10");
    ASSERT_EQUAL(Graph::allows_parallel_edges || Graph::is_directed, ok10);
    if (ok10) es.push_back(e10);
    auto map = graph[edge_property{}];
    graph[es[2]].s = "e21a";
    put(map, es[0], bundle(0, "e01b"));
    ASSERT_EQUAL("e01b", graph[es[0]].s);
    ASSERT_EQUAL("e02", graph[es[1]].s);
    ASSERT_EQUAL("e21a", get(map, es[2]).s);
    auto e_range = graph.edges();
    ASSERT_EQUAL_SETS(get_ends<Graph::is_directed>(es),
                      get_ends<Graph::is_directed>(std::vector<E>(e_range.begin(), e_range.end())));
    auto e02 = graph.find_edge(v0, v2);
    ASSERT_EQUAL_SETS(std::vector<V>{v0, v2}, {e02.source(), e02.target()});
    ASSERT_EQUAL(es[1], e02);
    if constexpr (TestAdjacency) {
        std::map<V, std::vector<V>> outs, ins;
        auto add_edge = [&](V u, V v) {
            outs[u].push_back(v);
            if constexpr (Graph::is_undirected) {
                if (u != v) outs[v].push_back(u);
            }
            if constexpr (Graph::is_bidirectional) {
                ins[v].push_back(u);
            }
        };
        add_edge(v0, v1);
        add_edge(v0, v2);
        add_edge(v2, v1);
        add_edge(v2, v2);
        if (ok01) add_edge(v0, v1);
        if (ok10) add_edge(v1, v0);
        for (auto v : {v0, v1, v2}) {
            ASSERT_EQUAL_SETS(outs[v], get_targets(graph.out_edges(v)));
            if constexpr (Graph::is_undirected) {
                ASSERT_EQUAL_SETS(outs[v], get_sources(graph.in_edges(v)));
            }
            if constexpr (Graph::is_bidirectional) {
                ASSERT_EQUAL_SETS(ins[v], get_sources(graph.in_edges(v)));
            }
        }
    };
    // TODO: uncomment when remove_edge is implemented in adjacency_list.
    // graph.remove_edge(e02);
    // ASSERT_EQUAL(3 + ok01 + ok10, graph.edge_count());
}
