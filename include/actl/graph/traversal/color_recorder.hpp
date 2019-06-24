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
struct color_recorder : map_wrapper_t<Map> {
    static_assert(std::is_same_v<typename map_traits<Map>::value_type, colors>);

    using vertex = typename map_traits<Map>::key_type;

    color_recorder(Map&& pm) : map_wrapper_t<Map>{std::move(pm)} {}

    void operator()(on_vertex_initialize, vertex u) { put(*this, u, colors::white); }
    bool operator()(is_vertex_discovered, vertex u) { return get(*this, u) != colors::white; }
    void operator()(on_vertex_discover, vertex u) { put(*this, u, colors::gray); }
    void operator()(on_vertex_finish, vertex u) { put(*this, u, colors::black); }
};

template <class Graph>
inline auto default_color_recorder(const Graph& graph) {
    return color_recorder{default_vertex_map<colors>(graph)};
}

}  // namespace ac
