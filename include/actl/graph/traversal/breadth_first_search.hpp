/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/always_false.hpp>
#include <actl/graph/events.hpp>
#include <actl/graph/traits.hpp>
#include <actl/util/component_set.hpp>
#include <queue>

namespace ac {

template <class... Components>
class breadth_first_search : public component_set<Components...> {
    using base_t = component_set<Components...>;
    using base_t::execute_all;

public:
    using base_t::base_t;

    template <class Graph, class Source, class VertexQueue = std::queue<vertex_t<Graph>>,
              class VertexPredicate = always_false>
    void operator()(const Graph& graph, const Source& source, VertexQueue&& queue = {},
                    VertexPredicate is_terminator = {}) {
        using V = vertex_t<Graph>;
        for (V u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        while (!queue.empty()) queue.pop();
        auto discover = [&](V u) {
            execute_all(on_vertex_discover{}, u);
            if (is_terminator(u)) {
                execute_all(on_search_finish{}, u);
                return false;
            }
            queue.push(u);
            return true;
        };
        if constexpr (std::is_same_v<Source, V>) {
            execute_all(on_search_start{}, source);
            if (!discover(source)) return;
        } else {
            for (V s : source) {
                execute_all(on_search_start{}, s);
                if (!discover(s)) return;
            }
        }
        while (!queue.empty()) {
            V u = queue.front();
            queue.pop();
            execute_all(on_vertex_start{}, u);
            for (auto e : graph.out_edges(u)) {
                V v = e.target();
                if (base_t::execute_first(is_vertex_discovered{}, v)) {
                    execute_all(on_non_tree_edge{}, e);
                } else {
                    execute_all(on_tree_edge_start{}, e);
                    if (!discover(v)) return;
                    execute_all(on_tree_edge_finish{}, e);
                }
            }
            execute_all(on_vertex_finish{}, u);
        }
        execute_all(on_search_finish{}, graph.null_vertex());
    }
};

template <class... Components>
breadth_first_search(Components&&...) -> breadth_first_search<Components...>;

}  // namespace ac
