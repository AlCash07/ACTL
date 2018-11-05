/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/default_property_map.hpp>
#include <actl/graph/events.hpp>
#include <type_traits>

namespace ac {

template <class Map>
struct discovered_flag : property_map_wrapper_t<Map> {
    static_assert(std::is_same_v<typename property_traits<Map>::value_type, bool>);
    using vertex = typename property_traits<Map>::key_type;

    void operator()(on_vertex_initialize, vertex u) { put(*this, u, false); }
    void operator()(on_vertex_discover, vertex u) { put(*this, u, true); }
    bool operator()(is_vertex_discovered, vertex u) { return get(*this, u); }
};

template <class Map>
inline discovered_flag<Map> make_discovered_flag(Map&& discovered) {
    return {std::forward<Map>(discovered)};
}

template <class Graph>
inline auto make_default_discovered_flag(const Graph& graph) {
    return make_discovered_flag(make_default_vertex_property_map<bool>(graph));
}

}  // namespace ac
