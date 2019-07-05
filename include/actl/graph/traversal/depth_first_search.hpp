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
#include <actl/util/type_traits.hpp>
#include <stack>

namespace ac {

template <class... Components>
class depth_first_search : public component_set<Components...> {
    using base_t = component_set<Components...>;
    using base_t::execute_all;

    // Recursive implementation to demonstrate dfs logic clearer.
    template <class Graph, class VertexPredicate>
    void recurse(const Graph& graph, typename Graph::vertex u, VertexPredicate is_terminator) {
        execute_all(on_vertex_discover{}, u);
        execute_all(on_vertex_examine{}, u);
        if (is_terminator(u)) return;
        for (auto e : graph.out_edges(u)) {
            auto v = e.target();
            execute_all(on_edge_examine{}, e);
            if (base_t::execute_first(is_vertex_discovered{}, v)) {
                execute_all(on_non_tree_edge{}, e);
            } else {
                execute_all(on_tree_edge{}, e);
                recurse(graph, v, is_terminator);
            }
            execute_all(on_edge_finish{}, e);
        }
        execute_all(on_vertex_finish{}, u);
    }

public:
    template <class Graph>
    struct stack_value {
        typename Graph::vertex vertex;
        typename Graph::out_edge_id out_edge;
    };

    using base_t::base_t;

    // Depth first search without initialization.
    template <class Graph, class Stack = std::stack<stack_value<Graph>>,
              class VertexPredicate = always_false>
    void visit(const Graph& graph, typename Graph::vertex u, Stack&& stack = {},
               VertexPredicate is_terminator = {}) {
        while (!stack.empty()) stack.pop();
        execute_all(on_vertex_start{}, u);
        auto it = graph.out_edges(u).begin();
        stack.push({u, it.id()});
        while (!stack.empty()) {
            u = stack.top().vertex;
            if (it == graph.out_edges(u).begin()) {
                execute_all(on_vertex_discover{}, u);
                execute_all(on_vertex_examine{}, u);
                if (is_terminator(u)) {
                    execute_all(on_search_finish{});
                    return;
                }
            }
            for (auto end = graph.out_edges(u).end();; ++it) {
                if (it == end) {
                    stack.pop();
                    if (!stack.empty()) {
                        it = typename Graph::out_edge_iterator{&graph, stack.top().vertex,
                                                               stack.top().out_edge};
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
                    stack.top().out_edge = it.id();
                    it = graph.out_edges(v).begin();
                    stack.push({v, it.id()});
                    break;
                }
            }
        }
        execute_all(on_search_finish{});
    }

    template <class Graph, class Stack = std::stack<stack_value<Graph>>,
              class VertexPredicate = always_false>
    void operator()(const Graph& graph, typename Graph::vertex s, Stack&& stack = {},
                    VertexPredicate is_terminator = {}) {
        for (auto u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        visit(graph, s, std::forward<Stack>(stack), is_terminator);
    }

    template <class Graph, class Stack = std::stack<stack_value<Graph>>,
              class VertexPredicate = always_false,
              enable_int_if<!std::is_same_v<remove_cvref_t<Stack>, typename Graph::vertex>> = 0>
    void operator()(const Graph& graph, Stack&& stack = {}, VertexPredicate is_terminator = {}) {
        for (auto u : graph.vertices()) execute_all(on_vertex_initialize{}, u);
        for (auto s : graph.vertices()) {
            if (!base_t::execute_first(is_vertex_discovered{}, s))
                visit(graph, s, std::forward<Stack>(stack), is_terminator);
        }
    }
};

template <class... Components>
depth_first_search(Components&&...) -> depth_first_search<Components...>;

}  // namespace ac
