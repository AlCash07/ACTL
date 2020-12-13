// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/graph/adjacency_list.hpp>
#include <actl/graph/connectivity/tarjan_strong_components.hpp>
#include <actl/map/generic_container.hpp>
#include "graph/connectivity/test_partition.hpp"

// 2 - 3 <- 7 - 8
//  \ / \    \ /
//   0   4    6
//    \ /    /
//     1 <- 5
TEST("basic") {
    adjacency_list<directed> graph;
    graph.add_edge(0, 1);
    graph.add_edge(0, 3);
    graph.add_edge(3, 2);
    graph.add_edge(2, 0);
    graph.add_edge(3, 4);
    graph.add_edge(4, 1);
    graph.add_edge(5, 1);
    graph.add_edge(7, 3);
    graph.add_edge(5, 6);
    graph.add_edge(6, 7);
    graph.add_edge(7, 8);
    graph.add_edge(8, 6);
    std::vector<int> components(9);
    find_strong_components(graph, components);
    test_partition({{0, 2, 3}, {1}, {4}, {5}, {6, 7, 8}}, components);
    for (auto e : graph.edges()) {
        ASSERT_TRUE(components[(size_t)e.source()] >= components[(size_t)e.target()]);
    }
}
