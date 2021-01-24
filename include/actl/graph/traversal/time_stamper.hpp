// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/default_map.hpp>
#include <actl/graph/traversal/vertex_initializer.hpp>

namespace ac {

// T can be a reference to share global time.
template <class Map, class T>
struct time_stamper {
    void operator()(on_vertex_start, map_key_t<Map> u) {
        put(map, u, time_now);
        ++time_now;
    }

    Map map;
    T time_now = 0;
};

template <class Map>
time_stamper(Map&&) -> time_stamper<Map, map_value_t<Map>>;

template <class Map, class T>
struct in_out_time_stamper : time_stamper<Map, T> {
    Map out_time;

    using time_stamper<Map, T>::operator();

    void operator()(on_vertex_finish, map_key_t<Map> u) {
        put(out_time, u, this->time_now);
    }
};

template <class Map>
in_out_time_stamper(Map&&) -> in_out_time_stamper<Map, map_value_t<Map>>;

template <class Map, class T>
vertex_initializer<time_stamper<Map, T>> make_time_stamper(Map&& in_time, T value) {
    return {{std::forward<Map>(in_time)}, value};
}

template <class Map>
auto make_in_out_time_stamper(Map&& in_time, Map&& out_time) {
    return in_out_time_stamper{{std::forward<Map>(in_time)}, std::forward<Map>(out_time)};
}

template <class Map, class T>
vertex_initializer<in_out_time_stamper<Map, T>> make_in_out_time_stamper(
    Map&& in_time, Map&& out_time, T value) //
{
    return {{{std::forward<Map>(in_time)}, std::forward<Map>(out_time)}, value};
}

template <class Graph>
auto make_default_time_stamper(const Graph& graph) {
    return make_time_stamper(make_default_vertex_map<int>(graph), -1);
}

} // namespace ac
