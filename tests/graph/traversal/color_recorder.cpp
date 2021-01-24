// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/breadth_first_search.hpp>
#include <actl/graph/traversal/color_recorder.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include "graph/sample_graphs.hpp"
#include "map/logging_map.hpp"
#include "test.hpp"

using Log = std::vector<std::pair<int, colors>>;

TEST_CASE("color_recorder bfs") {
    auto graph = sample_undirected_graph();
    Log log;
    auto map = logging_map{
        make_default_vertex_map<colors>(graph), std::back_inserter(log)};
    breadth_first_search{color_recorder{map}}(graph, 0);
    CHECK(
        Log{
            {0, colors::white},
            {1, colors::white},
            {2, colors::white},
            {3, colors::white},
            {4, colors::white},
            {5, colors::white},
            {0, colors::gray},
            {1, colors::gray},
            {3, colors::gray},
            {0, colors::black},
            {2, colors::gray},
            {1, colors::black},
            {4, colors::gray},
            {5, colors::gray},
            {3, colors::black},
            {2, colors::black},
            {4, colors::black},
            {5, colors::black},
        } == log);
}

TEST_CASE("color_recorder dfs") {
    auto graph = sample_undirected_graph();
    Log log;
    auto map = logging_map{
        make_default_vertex_map<colors>(graph), std::back_inserter(log)};
    depth_first_search{color_recorder{map}}(graph, 0);
    CHECK(
        Log{
            {0, colors::white},
            {1, colors::white},
            {2, colors::white},
            {3, colors::white},
            {4, colors::white},
            {5, colors::white},
            {0, colors::gray},
            {1, colors::gray},
            {2, colors::gray},
            {3, colors::gray},
            {4, colors::gray},
            {5, colors::gray},
            {5, colors::black},
            {4, colors::black},
            {3, colors::black},
            {2, colors::black},
            {1, colors::black},
            {0, colors::black},
        } == log);
}
