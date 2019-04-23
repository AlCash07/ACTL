/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/always_false.hpp>
#include <actl/graph/events.hpp>
#include <actl/util/component_set.hpp>
#include <queue>

namespace ac {

// TODO: rename into breadth_first_search when C++17 is fully supported.
template <class... Components>
class bfs : public component_set<Components...> {
    using base_t = component_set<Components...>;
    using base_t::execute_all;

public:
    using base_t::base_t;

    template <class Graph, class Source, class VertexQueue = std::queue<typename Graph::vertex>,
              class VertexPredicate = always_false>
    void operator()(const Graph& graph, const Source& source, VertexQueue&& queue = {},
                    VertexPredicate is_terminator = {}) {
        using vertex = typename Graph::vertex;
        for (vertex u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        while (!queue.empty()) queue.pop();
        auto enqueue = [&](vertex u) {
            execute_all(on_vertex_discover{}, u);
            if (is_terminator(u)) {
                execute_all(on_search_finish{});
                return false;
            }
            queue.push(u);
            return true;
        };
        if constexpr (std::is_same_v<Source, vertex>) {
            execute_all(on_vertex_start{}, source);
            if (!enqueue(source)) return;
        } else {
            for (vertex s : source) {
                execute_all(on_vertex_start{}, s);
                if (!enqueue(s)) return;
            }
        }
        while (!queue.empty()) {
            vertex u = queue.front();
            queue.pop();
            execute_all(on_vertex_examine{}, u);
            for (auto e : graph.out_edges(u)) {
                vertex v = e.target();
                execute_all(on_edge_examine{}, e);
                if (base_t::execute_first(is_vertex_discovered{}, v)) {
                    execute_all(on_non_tree_edge{}, e);
                } else {
                    execute_all(on_tree_edge{}, e);
                    if (!enqueue(v)) return;
                }
            }
            execute_all(on_vertex_finish{}, u);
        }
        execute_all(on_search_finish{});
    }
};

template <class... Components>
inline auto breadth_first_search(Components&&... components) {
    return bfs<Components...>{std::forward<Components>(components)...};
}

}  // namespace ac
