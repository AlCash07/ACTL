/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/all.hpp>
#include <actl/graph/vertex_list.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/test.hpp>
#include "bundle.hpp"

using namespace ac;

TEST("vertex_list::none") {
    vertex_list<> graph(3);
    ASSERT_EQUAL(std::make_pair(3, true), graph.try_add_vertex());
    ASSERT_EQUAL(4, graph.add_vertex());
    ASSERT_EQUAL(2, graph.nth_vertex(2));
    graph.remove_vertex(2);
    ASSERT_EQUAL(4, graph.vertex_count());
    int i = 0;
    for (auto v : graph.vertices()) {
        ASSERT_EQUAL(i, v);
        ++i;
    }
    ASSERT_TRUE(std::is_same_v<void, decltype(graph[0])>);
    auto pm = graph[vertex_property{}];
    ASSERT_TRUE(std::is_same_v<void, decltype(get(pm, 0))>);
    graph.clear();
    ASSERT_EQUAL(0, graph.vertex_count());
}

template <class VC>
void test_vertex_list() {
    using V = typename vertex_list<VC>::vertex;
    vertex_list<VC> graph;
    std::vector<V> vs;
    vs.push_back(graph.add_vertex(0, "v0"));
    vs.push_back(graph.add_vertex(1, "v1"));
    vs.push_back(graph.add_vertex(2, "v2"));
    ASSERT_EQUAL(3, graph.vertex_count());
    auto[v3, ok] = graph.try_add_vertex(1, "v3");
    ASSERT_EQUAL(!is_unique_associative_container_v<VC>, ok);
    if (ok) vs.push_back(v3);
    auto pm = graph[vertex_property{}];
    graph[vs[2]].s = "v2a";
    put(pm, vs[0], bundle(0, "v0a"));
    ASSERT_EQUAL("v0a", graph[vs[0]].s);
    ASSERT_EQUAL("v1", graph[vs[1]].s);
    ASSERT_EQUAL("v2a", get(pm, vs[2]).s);
    auto v_range = graph.vertices();
    std::vector<V> vs1(v_range.begin(), v_range.end());
    sort(vs);
    sort(vs1);
    ASSERT_EQUAL(vs, vs1);
    graph.remove_vertex(vs[1]);
    ASSERT_EQUAL(2 + ok, graph.vertex_count());
}

TEST("vertex_list::bundle") {
    test_vertex_list<std::vector<bundle>>();
    test_vertex_list<std::list<bundle>>();
    test_vertex_list<std::set<bundle>>();
    test_vertex_list<std::multiset<bundle>>();
    test_vertex_list<std::unordered_set<bundle>>();
    test_vertex_list<std::unordered_multiset<bundle>>();
}
