/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/container/dummy_container.hpp>
#include <actl/graph/detail/mimic_pair.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/numeric/hash.hpp>

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

    constexpr auto key() const { return std::pair{id_key(u), id_key(v)}; }

    bool operator < (const edge_vertices& rhs) const { return key() < rhs.key(); }

    bool operator == (const edge_vertices& rhs) const { return u == rhs.u && v == rhs.v; }

private:
    friend struct ac::hash_access;

    size_t hash() const { return hash_value(key()); }
};

template <class Dir, class V, class EC, class Selector>
struct edge_list_traits {
    using bundle    = value_type_t<EC>;
    using vertices  = edge_vertices<Dir, V, Selector>;
    using edge_data = mimic_pair<vertices, bundle, 1>;
    using container = rebind_container_t<EC, edge_data>;
};

template <class Dir, class V, class EC>
struct edge_list_traits<Dir, V, EC, none> {
    using bundle    = value_type_t<EC>;
    using vertices  = none;
    using container = dummy_container;
};

}  // namespace ac::detail
