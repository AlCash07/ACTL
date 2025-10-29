// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/hash_set.hpp>
#include <actl/graph/edge_list.hpp>
#include <actl/std/all.hpp>
#include "graph/test_edges.hpp"

TEST_CASE("edge_list none") {
    edge_list<directed, int> graph;
    auto e = graph.add_edge(2, 3);
    CHECK(2 == e.source());
    CHECK(3 == e.target());
    CHECK(std::is_same_v<void, decltype(graph[e])>);
}

template<typename Dir, typename EC>
void test_edge_list() {
    edge_list<Dir, int, EC> graph;
    test_edges(graph, 0, 1, 2);
}

TEST_CASE("edge_list bundle") {
    test_edge_list<directed, std::vector<bundle>>();
    test_edge_list<undirected, std::vector<bundle>>();
    test_edge_list<directed, std::set<bundle>>();
    test_edge_list<undirected, std::set<bundle>>();
    test_edge_list<directed, hash_multiset<bundle>>();
    test_edge_list<undirected, hash_multiset<bundle>>();
}
