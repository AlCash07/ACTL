/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/always_false.hpp>
#include <actl/graph/events.hpp>
#include <actl/type/component_set.hpp>
#include <stack>

namespace ac {

template <class... Components>
class dfs : public component_set<Components...> {
    using base_t = component_set<Components...>;
    using base_t::execute_all;

    // Recursive implementation to demonstrate dfs logic clearer.
    template <class Graph, class VertexPredicate>
    void recurse(const Graph& graph, typename Graph::vertex u, VertexPredicate terminator) {
        execute_all(on_vertex_discover{}, u);
        execute_all(on_vertex_examine{}, u);
        if (terminator(u)) return;
        for (auto e : graph.out_edges(u)) {
            auto v = e.target();
            execute_all(on_edge_examine{}, e);
            if (base_t::execute_first(is_vertex_discovered{}, v)) {
                execute_all(on_non_tree_edge{}, e);
            } else {
                execute_all(on_tree_edge{}, e);
                recurse(graph, v, terminator);
            }
            execute_all(on_edge_finish{}, e);
        }
        execute_all(on_vertex_finish{}, u);
    }

public:
    template <class Graph>
    using stack_value_t = std::pair<typename Graph::vertex, typename Graph::out_edge_id>;

    using base_t::base_t;

    // Depth first search without initialization.
    template <class Graph, class Stack = std::stack<stack_value_t<Graph>>,
              class VertexPredicate = always_false>
    void visit(const Graph& graph, typename Graph::vertex u, Stack&& stack = {},
               VertexPredicate terminator = {}) {
        while (!stack.empty()) stack.pop();
        execute_all(on_vertex_start{}, u);
        auto it = graph.out_edges(u).begin();
        stack.push({u, it.id()});
        while (!stack.empty()) {
            u = stack.top().first;
            if (it == graph.out_edges(u).begin()) {
                execute_all(on_vertex_discover{}, u);
                execute_all(on_vertex_examine{}, u);
                if (terminator(u)) {
                    execute_all(on_search_finish{});
                    return;
                }
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
                auto v = it->target();
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
        execute_all(on_search_finish{});
    }

    template <class Graph, class OutEdgeIteratorStack = std::stack<stack_value_t<Graph>>,
              class VertexPredicate = always_false>
    void operator()(const Graph& graph, typename Graph::vertex s, OutEdgeIteratorStack&& stack = {},
                    VertexPredicate terminator = {}) {
        for (typename Graph::vertex u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        visit(graph, s, std::forward<OutEdgeIteratorStack>(stack));
    }

    template <class Graph, class OutEdgeIteratorStack = std::stack<stack_value_t<Graph>>,
              class VertexPredicate = always_false>
    std::enable_if_t<!std::is_same_v<remove_cvref_t<OutEdgeIteratorStack>, typename Graph::vertex>>
    operator()(const Graph& graph, OutEdgeIteratorStack&& stack = {},
               VertexPredicate terminator = {}) {
        for (auto u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        for (auto s : graph.vertices()) {
            if (!base_t::execute_first(is_vertex_discovered{}, s))
                visit(graph, s, std::forward<OutEdgeIteratorStack>(stack));
        }
    }
};

template <class... Components>
inline auto depth_first_search(Components&&... components) {
    return dfs<Components...>(std::forward<Components>(components)...);
}

}  // namespace ac
