/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/events.hpp>
#include <actl/type/component_set.hpp>
#include <stack>

namespace ac {

template <class... Components>
class dfs : public component_set<Components...> {
    using base_t = component_set<Components...>;
    using base_t::execute_all;

    // Recursive implementation to demonstrate dfs logic better.
    template <class Graph, class V = typename Graph::vertex>
    void recurse(const Graph& graph, V u) {
        execute_all(on_vertex_discover{}, u);
        for (auto e : graph.out_edges(u)) {
            V v = e.target();
            execute_all(on_edge_examine{}, e);
            if (base_t::execute_first(is_vertex_discovered{}, v)) {
                execute_all(on_non_tree_edge{}, e);
            } else {
                execute_all(on_tree_edge{}, e);
                recurse(graph, v);
            }
            execute_all(on_edge_finish{}, e);
        }
        execute_all(on_vertex_finish{}, u);
    }

    template <class Graph, class Stack, class V = typename Graph::vertex>
    void impl(const Graph& graph, V u, Stack&& stack) {
        while (!stack.empty()) stack.pop();
        execute_all(on_vertex_start{}, u);
        auto it = graph.out_edges(u).begin();
        stack.push({u, it.id()});
        while (!stack.empty()) {
            u = stack.top().first;
            if (it == graph.out_edges(u).begin()) {
                execute_all(on_vertex_discover{}, u);
                execute_all(on_vertex_examine{}, u);
            }
            for (auto end = graph.out_edges(u).end();; ++it) {
                if (it == end) {
                    stack.pop();
                    if (!stack.empty()) {
                        it = typename Graph::out_edge_iterator(&graph, stack.top().first,
                                                               stack.top().second);
                        execute_all(on_edge_finish{}, *it);
                        ++it;
                    }
                    execute_all(on_vertex_finish{}, u);
                    break;
                }
                execute_all(on_edge_examine{}, *it);
                V v = it->target();
                if (base_t::execute_first(is_vertex_discovered{}, v)) {
                    execute_all(on_non_tree_edge{}, *it);
                    execute_all(on_edge_finish{}, *it);
                } else {
                    execute_all(on_tree_edge{}, *it);
                    stack.top().second = it.id();
                    it = graph.out_edges(v).begin();
                    stack.push({v, it.id()});
                    break;
                }
            }
        }
    }

public:
    template <class Graph>
    using stack_value_t = std::pair<typename Graph::vertex, typename Graph::out_edge_id>;

    using base_t::base_t;

    template <class Graph, class OutEdgeIteratorStack = std::stack<stack_value_t<Graph>>>
    void operator()(const Graph& graph, typename Graph::vertex s,
                    OutEdgeIteratorStack&& stack = {}) {
        using vertex = typename Graph::vertex;
        for (vertex u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        impl(graph, s, std::forward<OutEdgeIteratorStack>(stack));
    }

    template <class Graph, class OutEdgeIteratorStack = std::stack<stack_value_t<Graph>>>
    std::enable_if_t<!std::is_same_v<remove_cvref_t<OutEdgeIteratorStack>, typename Graph::vertex>>
    operator()(const Graph& graph, OutEdgeIteratorStack&& stack = {}) {
        for (auto u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        for (auto s : graph.vertices()) {
            if (!base_t::execute_first(is_vertex_discovered{}, s))
                impl(graph, s, std::forward<OutEdgeIteratorStack>(stack));
        }
    }
};

template <class... Components>
inline auto depth_first_search(Components&&... components) {
    return dfs<Components...>(std::forward<Components>(components)...);
}

}  // namespace ac
