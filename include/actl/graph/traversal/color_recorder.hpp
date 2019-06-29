/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/default_map.hpp>
#include <actl/graph/events.hpp>

namespace ac {

enum class colors : uint8_t { white, gray, black };

template <class Map>
struct color_recorder {
    static_assert(std::is_same_v<map_value_t<Map>, colors>);

    using vertex = map_key_t<Map>;

    void operator()(on_vertex_initialize, vertex u) { put(map, u, colors::white); }
    bool operator()(is_vertex_discovered, vertex u) { return get(map, u) != colors::white; }
    void operator()(on_vertex_discover, vertex u) { put(map, u, colors::gray); }
    void operator()(on_vertex_finish, vertex u) { put(map, u, colors::black); }

    Map map;
};

template <class Map>
color_recorder(Map&&) -> color_recorder<Map>;

template <class Graph>
inline auto make_default_color_recorder(const Graph& graph) {
    return color_recorder{make_default_vertex_map<colors>(graph)};
}

}  // namespace ac
