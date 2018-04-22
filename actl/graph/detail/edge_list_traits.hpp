/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/generic_container.hpp>
#include <actl/functions.hpp>
#include <actl/graph/detail/bundle_decorator.hpp>
#include <actl/graph/selectors.hpp>
#include <utility>

namespace ac::detail {

template <class Dir, class VId, class Selector>
struct edge_vertices {
    edge_vertices(VId, VId) {}
};

template <class VId>
struct edge_vertices<directed, VId, one_vertex> {
    VId v;

    edge_vertices(VId, VId v) : v(v) {}

    VId dst(VId) const { return v; }
};

template <class Dir, class VId>
struct edge_vertices<Dir, VId, one_vertex> {
    static_assert(std::is_same_v<VId, int>,
                  "undirected and bidirectional graphs are incompatible with one_vertex edge"
                  "selector and non-int vertex id");

    int x;

    edge_vertices(int u, int v) : x(u ^ v) {}

    int dst(int src) const { return src ^ x; }

    int other(int src) const { return dst(src); }
};

template <class Dir, class VId>
struct edge_vertices<Dir, VId, two_vertices> {
    VId u, v;

    edge_vertices(VId u, VId v) : u(u), v(v) {}

    VId dst(VId src) const {
        if constexpr (!std::is_same_v<Dir, undirected>) {
            return v;
        } else {
            return other(src);
        }
    }

    VId other(VId src) const {
        if constexpr (std::is_same_v<VId, int>) {
            return src ^ u ^ v;
        } else {
            return src == u ? v : u;
        }
    }

    constexpr auto key() const { return std::make_pair(get_id_key(u), get_id_key(v)); }

    bool operator < (const edge_vertices& rhs) const { return key() < rhs.key(); }

    bool operator == (const edge_vertices& rhs) const { return u == rhs.u && v == rhs.v; }
};

template <class Dir, class VId, class EC, class Selector>
struct edge_list_traits {
    using bundle    = value_type_t<EC>;
    using vertices  = edge_vertices<Dir, VId, Selector>;
    using edge_data = bundle_decorator<vertices, bundle, true>;
    using container = generic_container<typename container_traits<EC>::template rebind<edge_data>>;

    using out_edge_bundle = typename container::id;
};

template <class Dir, class VId, class EC>
struct edge_list_traits<Dir, VId, EC, none> {
    using bundle    = value_type_t<EC>;
    using vertices  = none;
    using container = generic_container<none, none, true>;  // Only maintains edge count.

    // Edge bundle is duplicated if graph is undirected or bidirectional, so it's immutable.
    using out_edge_bundle = std::conditional_t<std::is_same_v<Dir, directed>, bundle, const bundle>;
};

template <class VId, class EC, bool DiffId>
struct edge_id {
    using type     = typename EC::id;
    using iterator = typename EC::id_iterator;
};

template <class EC>
struct edge_id<typename EC::id, EC, true> {
    using type     = wrap_id<typename EC::id>;
    using iterator = wrap_id_iterator<typename EC::id_iterator>;
};

}  // namespace ac::detail

namespace std {

template <class Dir, class VId>
struct hash<ac::detail::edge_vertices<Dir, ac::two_vertices, VId>> {
    auto operator()(const ac::detail::edge_vertices<Dir, ac::two_vertices, VId>& arg) const {
        auto key = arg.key();
        return std::hash<ac::id_key_t<VId>>{}(ac::hash_combine(key.first, key.second));
    }
};

}  // namespace std
