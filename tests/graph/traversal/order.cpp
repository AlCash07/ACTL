/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/graph/traversal/order.hpp>
#include <actl/test.hpp>
#include "sample_graphs.hpp"

using namespace ac;

TEST("inorder") {
    auto graph = sample_undirected_tree();
    std::vector<int> order;
    print_inorder(graph, 0, std::back_inserter(order));
    ASSERT_EQUAL(std::vector<int>{1, 0, 3, 2, 6, 4, 2, 5}, order);
}

TEST("postorder") {
    auto graph = sample_undirected_tree();
    std::vector<int> order;
    print_postorder(graph, 0, std::back_inserter(order));
    ASSERT_EQUAL(std::vector<int>{1, 3, 6, 4, 5, 2, 0}, order);
}

TEST("preorder") {
    auto graph = sample_undirected_tree();
    std::vector<int> order;
    print_preorder(graph, 0, std::back_inserter(order));
    ASSERT_EQUAL(std::vector<int>{0, 1, 2, 3, 4, 6, 5}, order);
}

//   0   5
//  / \
// 1   2 6
// |\ /  |
// | 3   7
// |/    |
// 4     8
TEST("topological_sort") {
    adjacency_list<directed> graph;
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 3);
    graph.add_edge(1, 4);
    graph.add_edge(3, 4);
    graph.add_edge(6, 7);
    graph.add_edge(7, 8);
    int n = graph.vertex_count();
    std::vector<int> order;
    topological_sort(graph, std::back_inserter(order));
    ASSERT_EQUAL(n, (int)order.size());
    std::vector<int> idx(order.size());
    for (int i = 0; i < n; ++i) {
        idx[(size_t)order[(size_t)i]] = i;
    }
    for (auto e : graph.edges()) {
        ASSERT_TRUE(idx[(size_t)e.target()] < idx[(size_t)e.source()]);
    }
}
