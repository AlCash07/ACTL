/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/component/vertex_initializer.hpp>

namespace ac {

template <class Map>
struct distance_recorder {
    Map distance;

    void operator()(on_vertex_start, typename property_traits<Map>::key_type u) const {
        put(distance, u, 0);
    }

    template <class E>
    void operator()(on_tree_edge, E e) const {
        put(distance, e.target(), get(distance, e.source()) + 1);
    }
};

template <class Map>
inline distance_recorder<Map> record_distance(Map distance) {
    return {distance};
}

template <class Map, class T>
inline vertex_initializer<&distance_recorder<Map>::distance> record_distance(Map distance,
                                                                             T   value) {
    return {{distance}, value};
}

}  // namespace ac
