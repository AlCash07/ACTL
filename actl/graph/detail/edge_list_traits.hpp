/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/generic_container.hpp>
#include <actl/functions.hpp>
#include <actl/graph/detail/mimic_pair.hpp>
#include <actl/graph/selectors.hpp>
#include <utility>

namespace ac::detail {

template <class Dir, class V, class Selector>
struct edge_vertices {
    edge_vertices(V, V) {}
};

template <class V>
struct edge_vertices<directed, V, one_vertex> {
    V v;

    edge_vertices(V, V v) : v(v) {}

    V other(V) const { return v; }
};

template <class Dir, class V>
struct edge_vertices<Dir, V, one_vertex> {
    V x;

    edge_vertices(V u, V v) : x(u ^ v) {
        static_assert(std::is_same_v<V, int>,
                      "undirected and bidirectional graphs are incompatible with one_vertex edge "
                      "selector and non-int vertex id");
    }

    V other(V src) const { return src ^ x; }
};

template <class Dir, class V>
struct edge_vertices<Dir, V, two_vertices> {
    V u, v;

    edge_vertices(V u, V v) : u(u), v(v) {}

    V other(V src) const {
        if constexpr (std::is_same_v<V, int>) {
            return src ^ u ^ v;
        } else {
            return src == u ? v : u;
        }
    }

    constexpr auto key() const { return std::make_pair(get_id_key(u), get_id_key(v)); }

    bool operator < (const edge_vertices& rhs) const { return key() < rhs.key(); }

    bool operator == (const edge_vertices& rhs) const { return u == rhs.u && v == rhs.v; }
};

template <class Dir, class V, class EC, class Selector>
struct edge_list_traits {
    using bundle    = value_type_t<EC>;
    using vertices  = edge_vertices<Dir, V, Selector>;
    using edge_data = mimic_pair<vertices, bundle, 1>;
    using container = generic_container<typename container_traits<EC>::template rebind<edge_data>>;
};

template <class Dir, class V, class EC>
struct edge_list_traits<Dir, V, EC, none> {
    using bundle    = value_type_t<EC>;
    using vertices  = none;
    using container = generic_container<none, none, true>;  // Only maintains edge count.
};

}  // namespace ac::detail

namespace std {

template <class Dir, class V>
struct hash<ac::detail::edge_vertices<Dir, ac::two_vertices, V>> {
    auto operator()(const ac::detail::edge_vertices<Dir, ac::two_vertices, V>& arg) const {
        auto key = arg.key();
        return std::hash<ac::id_key_t<V>>{}(ac::hash_combine(key.first, key.second));
    }
};

}  // namespace std
