/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/graph/traversal/breadth_first_search.hpp>
#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/discovered_flag.hpp>
#include <actl/graph/traversal/predecessor_edge_recorder.hpp>
#include <actl/test.hpp>
#include "sample_graphs.hpp"

TEST("bfs") {
    auto graph = sample_undirected_graph();
    using E = decltype(graph)::edge;
    std::vector<E> pred((size_t)graph.vertex_count());
    breadth_first_search{predecessor_edge_recorder{pred.data()}, default_discovered_flag(graph)}(
        graph, 0);
    ASSERT_EQUAL(
        std::vector<E>{E{}, graph.find_edge(0, 1), graph.find_edge(1, 2), graph.find_edge(0, 3),
                       graph.find_edge(3, 4), graph.find_edge(3, 5)},
        pred);
}

TEST("dfs") {
    auto graph = sample_undirected_graph();
    using E = decltype(graph)::edge;
    std::vector<E> pred((size_t)graph.vertex_count());
    depth_first_search{predecessor_edge_recorder{pred.data()}, default_discovered_flag(graph)}(
        graph, 0);
    ASSERT_EQUAL(
        std::vector<E>{E{}, graph.find_edge(0, 1), graph.find_edge(1, 2), graph.find_edge(2, 3),
                       graph.find_edge(3, 4), graph.find_edge(4, 5)},
        pred);
}
