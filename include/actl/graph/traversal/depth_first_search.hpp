// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/invoke_tuple.hpp>
#include <actl/graph/detail/always_false.hpp>
#include <actl/graph/events.hpp>
#include <actl/graph/traits.hpp>
#include <actl/meta/type_traits.hpp>
#include <actl/std/stack.hpp>
#include <actl/std/tuple.hpp>

namespace ac {

template <class G, class V = vertex_t<G>, class OEI = out_edge_iterator_t<G>>
class dfs_context
{
    V u_;
    out_edge_t<G> oe_;

public:
    dfs_context(V u, OEI it, OEI) : u_{u}, oe_{it.id()} {}

    V vertex() const
    {
        return u_;
    }

    auto get(G const& g) const
    {
        return std::tuple{u_, OEI{&g, u_, oe_}, g.out_edges(u_).end()};
    }
};

template <class... Components>
class depth_first_search : std::tuple<Components...>
{
    using base_t = std::tuple<Components...>;

    base_t& base() noexcept
    {
        return *this;
    }

    // Recursive implementation to demonstrate dfs logic clearer.
    // Returns true immediately if terminator vertex was found.
    template <class Graph, class VertexPredicate>
    bool recurse(
        Graph const& graph, vertex_t<Graph> u, VertexPredicate is_terminator)
    {
        invoke_all_matching(base(), on_vertex_discover{}, u);
        invoke_all_matching(base(), on_vertex_start{}, u);
        if (is_terminator(u))
            return true;
        for (auto e : graph.out_edges(u))
        {
            auto v = e.target();
            if (invoke_first_matching(base(), is_vertex_discovered{}, v))
            {
                invoke_all_matching(base(), on_non_tree_edge{}, e);
            }
            else
            {
                invoke_all_matching(base(), on_tree_edge_start{}, e);
                if (recurse(graph, v, is_terminator))
                    return true;
                invoke_all_matching(base(), on_tree_edge_finish{}, e);
            }
        }
        invoke_all_matching(base(), on_vertex_finish{}, u);
        return false;
    }

public:
    using base_t::base_t;

    // Depth first search without initialization.
    template <
        class Graph,
        class Stack = std::stack<dfs_context<Graph>>,
        class VertexPredicate = always_false,
        class V = vertex_t<Graph>>
    void visit(
        Graph const& graph,
        V u,
        Stack&& stack = {},
        VertexPredicate is_terminator = {})
    {
        stack = {};
        invoke_all_matching(base(), on_search_start{}, u);
        out_edge_iterator_t<Graph> it, end;
        auto discover_vertex = [&]()
        {
            invoke_all_matching(base(), on_vertex_discover{}, u);
            invoke_all_matching(base(), on_vertex_start{}, u);
            if (is_terminator(u))
                return false;
            it = graph.out_edges(u).begin();
            end = graph.out_edges(u).end();
            return true;
        };
        bool ok = discover_vertex();
        while (ok)
        {
            if (it == end)
            {
                invoke_all_matching(base(), on_vertex_finish{}, u);
                if (stack.empty())
                    break;
                std::tie(u, it, end) = stack.top().get(graph);
                stack.pop();
                invoke_all_matching(base(), on_tree_edge_finish{}, *it);
                ++it;
            }
            else
            {
                V v = it->target();
                if (invoke_first_matching(base(), is_vertex_discovered{}, v))
                {
                    invoke_all_matching(base(), on_non_tree_edge{}, *it);
                    ++it;
                }
                else
                {
                    invoke_all_matching(base(), on_tree_edge_start{}, *it);
                    stack.emplace(u, it, end);
                    u = v;
                    ok = discover_vertex();
                }
            }
        }
        invoke_all_matching(base(), on_search_finish{}, u);
    }

    template <
        class Graph,
        class Stack = std::stack<dfs_context<Graph>>,
        class VertexPredicate = always_false>
    void operator()(
        Graph const& graph,
        vertex_t<Graph> s,
        Stack&& stack = {},
        VertexPredicate is_terminator = {})
    {
        for (auto u : graph.vertices())
            invoke_all_matching(base(), on_vertex_initialize{}, u);
        visit(graph, s, stack, is_terminator);
    }

    template <
        class Graph,
        class Stack = std::stack<dfs_context<Graph>>,
        class VertexPredicate = always_false>
        requires(!std::is_same_v<std::remove_cvref_t<Stack>, vertex_t<Graph>>)
    void operator()(
        Graph const& graph,
        Stack&& stack = {},
        VertexPredicate is_terminator = {})
    {
        for (auto u : graph.vertices())
            invoke_all_matching(base(), on_vertex_initialize{}, u);
        for (auto s : graph.vertices())
        {
            if (!invoke_first_matching(base(), is_vertex_discovered{}, s))
                visit(graph, s, stack, is_terminator);
        }
    }
};

template <class... Components>
depth_first_search(Components&&...) -> depth_first_search<Components...>;

} // namespace ac
