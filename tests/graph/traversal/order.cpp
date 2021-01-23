// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/graph/traversal/order.hpp>
#include "graph/sample_graphs.hpp"
#include "test.hpp"

TEST_CASE("inorder") {
    auto graph = sample_undirected_tree();
    std::vector<int> order;
    inorder(graph, 0, std::back_inserter(order));
    CHECK(std::vector<int>{1, 0, 3, 2, 6, 4, 2, 5} == order);
}

TEST_CASE("postorder") {
    auto graph = sample_undirected_tree();
    std::vector<int> order;
    postorder(graph, 0, std::back_inserter(order));
    CHECK(std::vector<int>{1, 3, 6, 4, 5, 2, 0} == order);
}

TEST_CASE("preorder") {
    auto graph = sample_undirected_tree();
    std::vector<int> order;
    preorder(graph, 0, std::back_inserter(order));
    CHECK(std::vector<int>{0, 1, 2, 3, 4, 6, 5} == order);
}

//   0   5
//  / \
// 1   2 6
// |\ /  |
// | 3   7
// |/    |
// 4     8
TEST_CASE("topological_sort") {
    adjacency_list<directed> graph;
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 3);
    graph.add_edge(1, 4);
    graph.add_edge(3, 4);
    graph.add_edge(6, 7);
    graph.add_edge(7, 8);
    ac::index n = graph.vertex_count();
    std::vector<int> order;
    topological_sort(graph, std::back_inserter(order));
    CHECK(n == (ac::index)order.size());
    std::vector<int> idx(order.size());
    for (int i = 0; i < n; ++i) {
        idx[(size_t)order[(size_t)i]] = i;
    }
    for (auto e : graph.edges()) {
        CHECK(idx[(size_t)e.target()] < idx[(size_t)e.source()]);
    }
}
