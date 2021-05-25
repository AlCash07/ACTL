// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/default_map.hpp>
#include <actl/graph/events.hpp>
#include <type_traits>

namespace ac {

template <class Map>
struct discovered_flag
{
    static_assert(std::is_same_v<map_value_t<Map>, bool>);

    using vertex = map_key_t<Map>;

    void operator()(on_vertex_initialize, vertex u)
    {
        put(map, u, false);
    }

    void operator()(on_vertex_discover, vertex u)
    {
        put(map, u, true);
    }

    bool operator()(is_vertex_discovered, vertex u)
    {
        return get(map, u);
    }

    Map map;
};

template <class Map>
discovered_flag(Map&&) -> discovered_flag<Map>;

template <class Graph>
auto make_default_discovered_flag(const Graph& graph)
{
    return discovered_flag{make_default_vertex_map<bool>(graph)};
}

} // namespace ac
