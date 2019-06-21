/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/graph/traversal/breadth_first_search.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/distance_recorder.hpp>
#include <actl/test.hpp>
#include "sample_graphs.hpp"

TEST("bfs") {
    auto graph = sample_undirected_graph();
    std::vector<int> dist((size_t)graph.vertex_count());
    breadth_first_search{make_distance_recorder(dist.data(), -1)}(graph, 0);
    ASSERT_EQUAL(std::vector<int>{0, 1, 2, 1, 2, 2}, dist);
}

TEST("dfs") {
    auto graph = sample_undirected_graph();
    std::vector<int> dist((size_t)graph.vertex_count());
    depth_first_search{make_distance_recorder(dist.data(), -1)}(graph, 0);
    ASSERT_EQUAL(std::vector<int>{0, 1, 2, 3, 4, 5}, dist);
}
