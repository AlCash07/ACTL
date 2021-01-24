// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/graph/adjacency_list.hpp>
#include <actl/graph/connectivity/bridges.hpp>
#include <actl/map/generic_container.hpp>
#include "graph/connectivity/test_partition.hpp"
#include "graph/edges_ends.hpp"

// 0 - 2 - 4   6
// | \   \ |
// 1 - 3   5 - 7
TEST_CASE("bridges simple") {
    using Graph = adjacency_list<undirected>;
    Graph graph;
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(0, 3);
    graph.add_edge(1, 3);
    graph.add_edge(2, 4);
    graph.add_edge(2, 5);
    graph.add_edge(4, 5);
    graph.add_edge(5, 7);
    std::vector<edge_t<Graph>> bridges;
    std::vector<int> components(8);
    find_bridges_and_components<false>(
        graph, std::back_inserter(bridges), components);
    CHECK_EQUAL_SETS({{0, 2}, {5, 7}}, get_ends<false>(bridges));
    test_partition({{0, 1, 3}, {2, 4, 5}, {6}, {7}}, components);
}

// 0 = 1 - 2 = 3 - 4
TEST_CASE("bridges with parallel edges") {
    using Graph = adjacency_list<undirected>;
    Graph graph;
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(2, 3);
    graph.add_edge(3, 4);
    std::vector<edge_t<Graph>> bridges;
    find_bridges(graph, std::back_inserter(bridges));
    CHECK_EQUAL_SETS({{1, 2}, {3, 4}}, get_ends<false>(bridges));
    std::vector<int> components(5);
    find_two_edge_connected_components(graph, components);
    test_partition({{0, 1}, {2, 3}, {4}}, components);
}
