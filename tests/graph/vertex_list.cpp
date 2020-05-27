/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/hash_set.hpp>
#include <actl/graph/vertex_list.hpp>
#include <actl/std/all.hpp>
#include "graph/test_vertices.hpp"

TEST("none") {
    vertex_list<> graph{3};
    ASSERT_EQUAL(std::pair{3, true}, graph.try_add_vertex());
    ASSERT_EQUAL(4, graph.add_vertex());
    ASSERT_EQUAL(2, graph.nth_vertex(2));
    graph.remove_vertex(2);
    ASSERT_EQUAL(4l, graph.vertex_count());
    int i = 0;
    for (auto v : graph.vertices()) {
        ASSERT_EQUAL(i, v);
        ++i;
    }
    ASSERT_TRUE(std::is_same_v<void, decltype(graph[0])>);
    graph = {};
    ASSERT_EQUAL(0l, graph.vertex_count());
}

template <class VC>
void test_vertex_list() {
    vertex_list<VC> graph;
    auto vs = test_vertices(graph);
    graph.remove_vertex(vs[1]);
    ASSERT_EQUAL((long)vs.size() - 1, graph.vertex_count());
}

TEST("bundle") {
    test_vertex_list<std::vector<bundle>>();
    test_vertex_list<std::list<bundle>>();
    test_vertex_list<std::set<bundle>>();
    test_vertex_list<std::multiset<bundle>>();
    test_vertex_list<hash_set<bundle>>();
    test_vertex_list<hash_multiset<bundle>>();
}
