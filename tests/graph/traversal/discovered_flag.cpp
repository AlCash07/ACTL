/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/graph/default_property_map.hpp>
#include <actl/graph/traversal/breadth_first_search.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/discovered_flag.hpp>
#include <actl/property_map/logging_property_map.hpp>
#include <actl/test.hpp>
#include "sample_graphs.hpp"

using Log = std::vector<std::pair<int, bool>>;

TEST("bfs") {
    auto graph = sample_undirected_graph();
    Log log;
    auto df = discovered_flag{
        logging_property_map{default_vertex_property_map<bool>(graph), std::back_inserter(log)}};
    breadth_first_search{df}(graph, 0);
    ASSERT_EQUAL(Log{{0, false},
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
                     {5, true}},
                 log);
}

TEST("dfs") {
    auto graph = sample_undirected_graph();
    Log log;
    auto df = discovered_flag{
        logging_property_map{default_vertex_property_map<bool>(graph), std::back_inserter(log)}};
    depth_first_search{df}(graph, 0);
    ASSERT_EQUAL(Log{{0, false},
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
                     {5, true}},
                 log);
}
