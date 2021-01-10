// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/graph/traversal/depth_first_search.hpp>
#include <actl/graph/traversal/time_stamper.hpp>
#include "graph/sample_graphs.hpp"
#include "test.hpp"

TEST_CASE("time_stamper in") {
    auto graph = sample_undirected_graph();
    size_t n = (size_t)graph.vertex_count();
    std::vector<int> tin(n);
    depth_first_search{make_time_stamper(tin, -1)}(graph, 0);
    CHECK(std::vector<int>{0, 1, 2, 3, 4, 5} == tin);
}

TEST_CASE("time_stamper in and out") {
    auto graph = sample_undirected_tree();
    size_t n = (size_t)graph.vertex_count();
    std::vector<int> tin(n), tout(n);
    depth_first_search{make_in_out_time_stamper(tin, tout, -1)}(graph, 0);
    CHECK(std::vector<int>{0, 1, 2, 3, 4, 6, 5} == tin);
    CHECK(std::vector<int>{7, 2, 7, 4, 6, 7, 6} == tout);
}
