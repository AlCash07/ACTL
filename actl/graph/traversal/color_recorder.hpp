/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/default_property_map.hpp>
#include <actl/graph/events.hpp>

namespace ac {

enum class colors : uint8_t { white, gray, black };

template <class Map>
struct color_recorder : property_map_wrapper_t<Map> {
    static_assert(std::is_same_v<typename property_traits<Map>::value_type, colors>);

    using vertex = typename property_traits<Map>::key_type;

    void operator()(on_vertex_initialize, vertex u) const { put(*this, u, colors::white); }
    bool operator()(is_vertex_discovered, vertex u) const { return get(*this, u) != colors::white; }
    void operator()(on_vertex_discover, vertex u) const { put(*this, u, colors::gray); }
    void operator()(on_vertex_finish, vertex u) const { put(*this, u, colors::black); }
};

template <class Map>
inline color_recorder<Map> make_color_recorder(Map&& color) {
    return {std::forward<Map>(color)};
}

template <class Graph>
inline auto default_color_recorder(const Graph& graph) {
    return make_color_recorder(default_vertex_property_map<colors>(graph));
}

}  // namespace ac
