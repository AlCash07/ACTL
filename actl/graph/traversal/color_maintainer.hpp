/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/events.hpp>
#include <actl/property_map/property_map.hpp>

namespace ac {

enum class colors : uint8_t { white, gray, black };

template <class Map>
struct color_maintainer {
    static_assert(std::is_same_v<typename property_traits<Map>::value_type, colors>);

    using vertex = typename property_traits<Map>::key_type;

    Map color;

    void operator()(on_vertex_initialize, vertex u) const { put(color, u, colors::white); }

    bool operator()(is_vertex_discovered, vertex u) const { return get(color, u) != colors::white; }

    void operator()(on_vertex_discover, vertex u) const { put(color, u, colors::gray); }

    void operator()(on_vertex_finish, vertex u) const { put(color, u, colors::black); }
};

template <class Map>
inline color_maintainer<Map> maintain_colors(Map color) {
    return {color};
}

}  // namespace ac
