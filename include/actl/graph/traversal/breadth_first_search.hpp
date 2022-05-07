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
#include <queue>

namespace ac {

template <class... Components>
class breadth_first_search : std::tuple<Components...>
{
    using base_t = std::tuple<Components...>;

    base_t& base() noexcept
    {
        return *this;
    }

public:
    using base_t::base_t;

    template <
        class Graph,
        class Source,
        class VertexQueue = std::queue<vertex_t<Graph>>,
        class VertexPredicate = always_false>
    void operator()(
        Graph const& graph,
        Source const& source,
        VertexQueue&& queue = {},
        VertexPredicate is_terminator = {})
    {
        using V = vertex_t<Graph>;
        for (V u : graph.vertices())
            invoke_all_matching(base(), on_vertex_initialize{}, u);
        while (!queue.empty())
            queue.pop();
        auto discover = [&](V u)
        {
            invoke_all_matching(base(), on_vertex_discover{}, u);
            if (is_terminator(u))
            {
                invoke_all_matching(base(), on_search_finish{}, u);
                return false;
            }
            queue.push(u);
            return true;
        };
        if constexpr (std::is_same_v<Source, V>)
        {
            invoke_all_matching(base(), on_search_start{}, source);
            if (!discover(source))
                return;
        }
        else
        {
            for (V s : source)
            {
                invoke_all_matching(base(), on_search_start{}, s);
                if (!discover(s))
                    return;
            }
        }
        while (!queue.empty())
        {
            V u = queue.front();
            queue.pop();
            invoke_all_matching(base(), on_vertex_start{}, u);
            for (auto e : graph.out_edges(u))
            {
                V v = e.target();
                if (invoke_first_matching(base(), is_vertex_discovered{}, v))
                {
                    invoke_all_matching(base(), on_non_tree_edge{}, e);
                }
                else
                {
                    invoke_all_matching(base(), on_tree_edge_start{}, e);
                    if (!discover(v))
                        return;
                    invoke_all_matching(base(), on_tree_edge_finish{}, e);
                }
            }
            invoke_all_matching(base(), on_vertex_finish{}, u);
        }
        invoke_all_matching(base(), on_search_finish{}, graph.null_vertex());
    }
};

template <class... Components>
breadth_first_search(Components&&...) -> breadth_first_search<Components...>;

} // namespace ac
