/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/graph/adjacency_list.hpp>
#include <actl/graph/connectivity/connected_components.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/test.hpp>

// 1 - 0 - 7  6
// |   |
// 2 - 3  4 - 5
TEST("basic") {
    adjacency_list<undirected> graph;
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(0, 7);
    graph.add_edge(4, 5);
    std::vector<int> c(8);
    depth_first_search(make_connected_component_recorder(c.data(), -1))(graph);
    std::vector<std::vector<size_t>> expected{{0, 1, 2, 3, 7}, {4, 5}, {6}};
    for (size_t i = 0; i < expected.size(); ++i) {
        for (auto v : expected[i]) {
            ASSERT_EQUAL(c[expected[i][0]], c[v]);
        }
        for (size_t j = 0; j < expected.size(); ++j) {
            if (i != j) ASSERT_NOT_EQUAL(c[expected[i][0]], c[expected[j][0]]);
        }
    }
}
