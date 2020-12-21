// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "graph/sample_graphs.hpp"
#include "test.hpp"
#include <actl/graph/traversal/breadth_first_search.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/predecessor_recorder.hpp>

TEST_CASE("predecessor_recorder bfs") {
    auto graph = sample_undirected_graph();
    std::vector<int> pred((size_t)graph.vertex_count());
    breadth_first_search{make_predecessor_recorder(pred, -1)}(graph, 0);
    CHECK(std::vector<int>{0, 0, 1, 0, 3, 3} == pred);
}

TEST_CASE("predecessor_recorder dfs") {
    auto graph = sample_undirected_graph();
    std::vector<int> pred((size_t)graph.vertex_count());
    depth_first_search{make_predecessor_recorder(pred, -1)}(graph, 0);
    CHECK(std::vector<int>{0, 0, 1, 2, 3, 4} == pred);
}
