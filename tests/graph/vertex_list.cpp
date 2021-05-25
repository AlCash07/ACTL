// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/hash_set.hpp>
#include <actl/graph/vertex_list.hpp>
#include <actl/std/all.hpp>
#include "graph/test_vertices.hpp"

TEST_CASE("none")
{
    vertex_list<> graph{3};
    CHECK(std::pair{3, true} == graph.try_add_vertex());
    CHECK(4 == graph.add_vertex());
    CHECK(2 == graph.nth_vertex(2));
    graph.remove_vertex(2);
    CHECK(4l == graph.vertex_count());
    int i = 0;
    for (auto v : graph.vertices())
    {
        CHECK(i == v);
        ++i;
    }
    CHECK(std::is_same_v<void, decltype(graph[0])>);
    graph = {};
    CHECK(0l == graph.vertex_count());
}

template <class VC>
void test_vertex_list()
{
    vertex_list<VC> graph;
    auto vs = test_vertices(graph);
    graph.remove_vertex(vs[1]);
    CHECK((long)vs.size() - 1 == graph.vertex_count());
}

TEST_CASE("bundle")
{
    test_vertex_list<std::vector<bundle>>();
    test_vertex_list<std::list<bundle>>();
    test_vertex_list<std::set<bundle>>();
    test_vertex_list<std::multiset<bundle>>();
    test_vertex_list<hash_set<bundle>>();
    test_vertex_list<hash_multiset<bundle>>();
}
