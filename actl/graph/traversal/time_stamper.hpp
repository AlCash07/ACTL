/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/default_property_map.hpp>
#include <actl/graph/traversal/vertex_initializer.hpp>

namespace ac {

// T can be a reference to share global time.
template <class Map, class T = typename property_traits<Map>::value_type>
struct time_stamper : property_map_wrapper_t<Map> {
    T time;

    void operator()(on_vertex_examine, typename property_traits<Map>::key_type u) const {
        put(*this, u, time);
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
inline time_stamper<Map> make_time_stamper(Map in_time) {
    return {std::forward<Map>(in_time)};
}

template <class Map, class T>
inline vertex_initializer<time_stamper<Map>> make_time_stamper(Map&& in_time, T value) {
    return {{std::forward<Map>(in_time)}, value};
}

template <class Map>
inline time_stamper<Map> make_in_out_time_stamper(Map&& in_time, Map&& out_time) {
    return {{std::forward<Map>(in_time)}, std::forward<Map>(out_time)};
}

template <class Graph>
inline auto default_time_stamper(const Graph& graph) {
    return make_time_stamper(default_vertex_property_map<int>(graph), -1);
}

}  // namespace ac
