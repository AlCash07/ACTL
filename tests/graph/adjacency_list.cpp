/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/hash_set.hpp>
#include <actl/graph/adjacency_list.hpp>
#include <actl/std/all.hpp>
#include "test_edges.hpp"
#include "test_vertices.hpp"

TEST("none") {
    adjacency_list<directed> graph(2);
    graph.add_edge(0, 2);
    graph.add_vertex();
    ASSERT_EQUAL(4, graph.vertex_count());
    ASSERT_EQUAL(1, graph.edge_count());
}

template <class Dir, class OEC, class EC, class VC>
void test_adjacency_list() {
    adjacency_list<Dir, OEC, EC, VC> graph;
    auto vs = test_vertices(graph);
    test_edges<true>(graph, vs[0], vs[1], vs[2]);
}

template <class Dir>
void test_adjacency_list1() {
    test_adjacency_list<Dir, std::set<bundle>, none, std::vector<bundle>>();
    test_adjacency_list<Dir, hash_multiset<bundle>, std::list<edge_property>, std::set<bundle>>();
    test_adjacency_list<Dir, hash_set<bundle>, std::vector<one_vertex>, std::vector<bundle>>();
    test_adjacency_list<Dir, std::vector<bundle>, std::vector<two_vertices>,
                        hash_multiset<bundle>>();
}

TEST("bundle") {
    test_adjacency_list1<directed>();
    test_adjacency_list1<undirected>();
    test_adjacency_list1<bidirectional>();
}
