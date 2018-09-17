/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/all.hpp>
#include <actl/graph/edge_list.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/test.hpp>
#include "bundle.hpp"

using namespace ac;

TEST("edge_list::none") {
    edge_list<directed, int> graph;
    auto e = graph.add_edge(2, 3);
    ASSERT_EQUAL(2, e.source());
    ASSERT_EQUAL(3, e.target());
    ASSERT_TRUE(std::is_same_v<void, decltype(graph[e])>);
    auto pm = graph[edge_property{}];
    ASSERT_TRUE(std::is_same_v<void, decltype(get(pm, e))>);
}

template <class Dir, class EC>
void test_edge_list() {
    using E = typename edge_list<Dir, int, EC>::edge;
    edge_list<Dir, int, EC> graph;
    std::vector<E> es;
    es.push_back(graph.add_edge(0, 1, 0, "e01"));
    auto e02 = graph.add_edge(0, 2, 1, "e02");
    es.push_back(e02);
    es.push_back(graph.add_edge(2, 1, 2, "e21"));
    auto[e01, ok01] = graph.try_add_edge(0, 1, 3, "e01a");
    ASSERT_EQUAL(!is_unique_associative_container_v<EC>, ok01);
    if (ok01) es.push_back(e01);
    auto[e10, ok10] = graph.try_add_edge(1, 0, 4, "e10");
    ASSERT_EQUAL(!is_unique_associative_container_v<EC> || std::is_same_v<Dir, directed>, ok10);
    if (ok10) es.push_back(e10);
    auto pm = graph[edge_property{}];
    graph[es[2]].s = "e21a";
    put(pm, es[0], bundle(0, "e01b"));
    ASSERT_EQUAL("e01b", graph[es[0]].s);
    ASSERT_EQUAL("e21a", get(pm, es[2]).s);
    auto e_range = graph.edges();
    std::vector<E> es1(e_range.begin(), e_range.end());
    sort(es);
    sort(es1);
    ASSERT_EQUAL(es, es1);
    auto e02f = graph.find_edge(0, 2);
    ASSERT_EQUAL(0, e02f.source());
    ASSERT_EQUAL(2, e02f.target());
    ASSERT_EQUAL(e02, e02f);
    graph.remove_edge(e02);
    ASSERT_EQUAL(2 + ok01 + ok10, graph.edge_count());
}

TEST("edge_list::bundle") {
    test_edge_list<directed, std::vector<bundle>>();
    test_edge_list<undirected, std::vector<bundle>>();
    test_edge_list<directed, std::set<bundle>>();
    test_edge_list<undirected, std::set<bundle>>();
    test_edge_list<directed, std::unordered_multiset<bundle>>();
    test_edge_list<undirected, std::unordered_multiset<bundle>>();
}
