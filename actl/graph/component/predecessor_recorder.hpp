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
struct predecessor_recorder {
    Map predecessor;

    void operator()(on_vertex_start, typename property_traits<Map>::key_type u) const {
        put(predecessor, u, u);
    }

    template <class E>
    void operator()(on_tree_edge, E e) const {
        put(predecessor, e.target(), e.source());
    }

    template <class E>
    void operator()(on_edge_relaxed, E e) const {
        operator()(on_tree_edge{}, e);
    }
};

template <class Map>
inline predecessor_recorder<Map> record_predecessor(Map predecessor) {
    return {predecessor};
}

template <class Map, class T>
inline vertex_initializer<&predecessor_recorder<Map>::predecessor> record_predecessor(
    Map predecessor, T value) {
    return {{predecessor}, value};
}

}  // namespace ac
