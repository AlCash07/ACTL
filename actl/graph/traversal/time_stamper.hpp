/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/traversal/vertex_initializer.hpp>

namespace ac {

// T can be a reference to share global time.
template <class Map, class T = typename property_traits<Map>::value_type>
struct time_stamper {
    Map in_time;
    T   time;

    void operator()(on_vertex_examine, typename property_traits<Map>::key_type u) const {
        put(in_time, u, time);
        ++time;
    }
};

template <class Map, class T>
struct in_out_time_stamper : time_stamper<Map, T> {
    Map out_time;

    using time_stamper<Map, T>::operator();

    void operator()(on_vertex_finish, typename property_traits<Map>::key_type u) const {
        put(out_time, u, time);
    }
};

template <class Map>
inline time_stamper<Map> stamp_time(Map in_time) {
    return {in_time};
}

template <class Map, class T>
inline vertex_initializer<&time_stamper<Map>::in_time> stamp_time(Map in_time, T value) {
    return {{in_time}, value};
}

template <class Map>
inline time_stamper<Map> stamp_in_out_time(Map in_time, Map out_time) {
    return {{in_time}, out_time};
}

}  // namespace ac
