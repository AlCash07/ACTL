// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/breadth_first_search.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/discovered_flag.hpp>
#include "graph/sample_graphs.hpp"
#include "map/logging_map.hpp"
#include "test.hpp"

using Log = std::vector<std::pair<int, bool>>;

TEST_CASE("discovered_flag bfs") {
    auto graph = sample_undirected_graph();
    Log log;
    auto map = logging_map{make_default_vertex_map<bool>(graph), std::back_inserter(log)};
    breadth_first_search{discovered_flag{map}}(graph, 0);
    CHECK(Log{{0, false},
              {1, false},
              {2, false},
              {3, false},
              {4, false},
              {5, false},
              {0, true},
              {1, true},
              {3, true},
              {2, true},
              {4, true},
              {5, true}} == log);
}

TEST_CASE("discovered_flag dfs") {
    auto graph = sample_undirected_graph();
    Log log;
    auto map = logging_map{make_default_vertex_map<bool>(graph), std::back_inserter(log)};
    depth_first_search{discovered_flag{map}}(graph, 0);
    CHECK(Log{{0, false},
              {1, false},
              {2, false},
              {3, false},
              {4, false},
              {5, false},
              {0, true},
              {1, true},
              {2, true},
              {3, true},
              {4, true},
              {5, true}} == log);
}
