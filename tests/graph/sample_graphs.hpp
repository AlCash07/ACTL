// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/adjacency_list.hpp>

// 1 - 0
// |   |
// 2 - 3 - 4
//     | /
//     5
inline auto sample_undirected_graph() {
    ac::adjacency_list<ac::undirected> al;
    al.add_edge(0, 1);
    al.add_edge(1, 2);
    al.add_edge(2, 3);
    al.add_edge(3, 0);
    al.add_edge(3, 4);
    al.add_edge(3, 5);
    al.add_edge(4, 5);
    return al;
}

//   0
//  / \
// 1   2
//    /|\
//   3 4 5
//     |
//     6
inline auto sample_undirected_tree() {
    ac::adjacency_list<ac::undirected> al;
    al.add_edge(0, 1);
    al.add_edge(0, 2);
    al.add_edge(2, 3);
    al.add_edge(2, 4);
    al.add_edge(2, 5);
    al.add_edge(4, 6);
    return al;
}
