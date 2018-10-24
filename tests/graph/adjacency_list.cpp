/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/all.hpp>
#include <actl/graph/adjacency_list.hpp>
#include "test_edges.hpp"
#include "test_vertices.hpp"

using namespace ac;

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

template <class Dir, class OEC, class EC>
void test_adjacency_list3() {
    test_adjacency_list<Dir, OEC, EC, std::vector<bundle>>();
    if constexpr (std::is_same_v<Dir, directed> || !std::is_same_v<value_type_t<EC>, one_vertex>) {
        test_adjacency_list<Dir, OEC, EC, std::set<bundle>>();
        // TODO: fix the use of undeclared type as a template parameter and uncomment this line.
        // test_adjacency_list<Dir, OEC, EC, std::unordered_multiset<bundle>>();
    }
}

template <class Dir, class OEC>
void test_adjacency_list2() {
    test_adjacency_list3<Dir, OEC, none>();
    test_adjacency_list3<Dir, OEC, std::list<edge_property>>();
    test_adjacency_list3<Dir, OEC, std::vector<one_vertex>>();
    test_adjacency_list3<Dir, OEC, std::vector<two_vertices>>();
}

template <class Dir>
void test_adjacency_list1() {
    test_adjacency_list2<Dir, std::set<bundle>>();
    test_adjacency_list2<Dir, std::unordered_multiset<bundle>>();
    test_adjacency_list2<Dir, std::vector<bundle>>();
}

TEST("bundle") {
    test_adjacency_list1<directed>();
    test_adjacency_list1<undirected>();
    test_adjacency_list1<bidirectional>();
}
