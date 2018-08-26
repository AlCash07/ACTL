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
struct predecessor_edge_recorder : component {
    Map predecessor_edge;

    template <class E>
    void operator()(on_tree_edge, E e) const {
        put(predecessor_edge, e.target(), e);
    }

    template <class E>
    void operator()(on_edge_relaxed, E e) const {
        operator()(on_tree_edge{}, e);
    }
};

template <class Map>
inline predecessor_edge_recorder<Map> record_predecessor_edge(Map predecessor_edge) {
    return {{}, predecessor_edge};
}

}  // namespace ac
