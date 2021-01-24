// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/graph/adjacency_list.hpp>
#include <actl/graph/connectivity/connected_components.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include "graph/connectivity/test_partition.hpp"

// 1 - 0 - 7   6
// |   |
// 2 - 3   4 - 5
TEST_CASE("connected components basic") {
    adjacency_list<undirected> graph;
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(0, 7);
    graph.add_edge(4, 5);
    std::vector<int> components(8);
    depth_first_search(make_connected_component_recorder(components, -1))(
        graph);
    test_partition({{0, 1, 2, 3, 7}, {4, 5}, {6}}, components);
}
