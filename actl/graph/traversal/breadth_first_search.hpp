/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/events.hpp>
#include <actl/type/algorithm.hpp>

namespace ac {

// TODO: rename into breadth_first_search when C++17 is fully supported.
template <class... Components>
class bfs : public algorithm<Components...> {
    using base_t = algorithm<Components...>;
    using base_t::execute_all;

public:
    using base_t::base_t;

    template <class Graph, class Source, class Queue>
    void operator()(const Graph& graph, const Source& source, Queue&& q) {
        using vertex = typename Graph::vertex;
        for (vertex u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        auto start = [this, &q](vertex s) {
            q.push(s);
            execute_all(on_vertex_start{}, s);
            execute_all(on_vertex_discover{}, s);
        };
        if constexpr (std::is_same_v<Source, vertex>) {
            start(source);
        } else {
            for (vertex s : source) start(s);
        }
        while (!q.empty()) {
            vertex u = q.top();
            q.pop();
            execute_all(on_vertex_examine{}, u);
            for (auto e : graph.out_edges(u)) {
                vertex v = e.target();
                execute_all(on_edge_examine{}, e);
                if (base_t::execute_one(is_vertex_discovered{}, v)) {
                    execute_all(on_non_tree_edge{}, e);
                } else {
                    q.push(v);
                    execute_all(on_tree_edge{}, e);
                    execute_all(on_vertex_discover{}, v);
                }
            }
            execute_all(on_vertex_finish{}, u);
        }
    }
};

template <class... Components>
inline auto breadth_first_search(Components&&... components) {
    return bfs<Components...>{std::forward<Components>(components)...};
}

}  // namespace ac
