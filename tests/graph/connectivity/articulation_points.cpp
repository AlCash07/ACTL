/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/graph/adjacency_list.hpp>
#include <actl/graph/connectivity/articulation_points.hpp>
#include <actl/map/generic_container.hpp>
#include <actl/map/pair_associative_container.hpp>
#include <actl/std/map.hpp>
#include "graph/connectivity/test_partition.hpp"

using Graph = adjacency_list<undirected, std::vector<none>, std::vector<one_vertex>>;

// 1 - 0 - 3 - 4
//  \ /     \ /
//   2   6 - 5
TEST("simple") {
    Graph graph;
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 0);
    graph.add_edge(0, 3);
    graph.add_edge(3, 4);
    graph.add_edge(4, 5);
    graph.add_edge(5, 6);
    graph.add_edge(5, 3);
    std::map<edge_t<Graph>, int> component_map;
    std::vector<bool> is_articulation(7);
    find_articulation_points_and_components(graph, is_articulation, component_map);
    ASSERT_EQUAL_RANGES(std::vector{true, false, false, true, false, true, false}, is_articulation);
    std::vector<int> components(8);
    for (auto [edge, value] : component_map) {
        components[(size_t)edge.bundle()] = value;
    }
    test_partition({{0, 1, 2}, {3}, {4, 5, 7}, {6}}, components);
}

// 1 = 0 = 3
//  \ /
//   2   4 - 5
TEST("parallel edges") {
    Graph graph;
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 0);
    graph.add_edge(0, 3);
    graph.add_edge(0, 3);
    graph.add_edge(4, 5);
    std::map<edge_t<Graph>, int> component_map;
    std::vector<bool> is_articulation(6);
    find_articulation_points_and_components(graph, is_articulation, component_map);
    ASSERT_EQUAL_RANGES(std::vector{true, false, false, false, false, false}, is_articulation);
    std::vector<int> components(7);
    for (auto [edge, value] : component_map) {
        components[(size_t)edge.bundle()] = value;
    }
    test_partition({{0, 1, 2, 3}, {4, 5}, {6}}, components);
}
