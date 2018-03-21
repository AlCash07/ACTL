/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/container_traits.hpp>
#include <actl/functions.hpp>
#include <actl/graph/detail/bundle_decorator.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/type/none.hpp>
#include <utility>

namespace ac::detail {

template <class Dir, class Selector, class VID>
struct edge_vertices;

template <class Dir, class VID>
struct edge_vertices<Dir, edge_property, VID> {
    constexpr edge_vertices(VID, VID) {}
};

template <class VID>
struct edge_vertices<directed, one_vertex, VID> {
    VID v;

    edge_vertices(VID, VID v) : v(v) {}

    VID dst(VID) const { return v; }
};

template <class Dir, class VID>
struct edge_vertices<Dir, one_vertex, VID> {
    static_assert(std::is_same_v<VID, int>,
                  "undirected and bidirectional graphs are incompatible with one_vertex edge"
                  "selector and non-int vertex id");

    int x;

    edge_vertices(int u, int v) : x(u ^ v) {}

    int dst(int src) const { return src ^ x; }

    int other(int src) const { return dst(src); }
};

template <class Dir, class VID>
struct edge_vertices<Dir, two_vertices, VID> {
    VID u, v;

    edge_vertices(VID u, VID v) : u(u), v(v) {}

    VID dst(VID src) const {
        if constexpr (!std::is_same_v<Dir, undirected>) {
            return v;
        } else {
            return other(src);
        }
    }

    VID other(VID src) const {
        if constexpr (std::is_same_v<VID, int>) {
            return src ^ u ^ v;
        } else {
            return src == u ? v : u;
        }
    }

    auto key() const { return std::make_pair(get_id_key(u), get_id_key(v)); }

    friend bool operator < (const edge_vertices& lhs, const edge_vertices& rhs) {
        return lhs.key() < rhs.key();
    }

    friend bool operator == (const edge_vertices& lhs, const edge_vertices& rhs) {
        return lhs.u == rhs.u && lhs.v == rhs.v;
    }
};

template <class Dir, class EC, class VID, class Selector = two_vertices>
class edge_list {
    using edge_vertices  = edge_vertices<Dir, Selector, VID>;
    using edge_data      = bundle_decorator<edge_vertices, edge_property>;
    using edge_container = typename container_traits<EC>::template type<edge_data>;

    edge_container edges_;

public:
    static_assert(!is_associative<EC>::value || std::is_same_v<Selector, two_vertices>,
                  "associative edge list must have two_vertices selector");

    using edge_id = container_id_t<edge_container>;
    // I was too lazy to make a separate class for this typedef.
    using out_edge_bundle = edge_id;

    int size() const { return static_cast<int>(edges_.size()); }

    edge_id edge(VID u, VID v) const { return find(edges_, edge_vertices(u, v)); }

    template <class... Ts>
    auto add(VID u, VID v, Ts&&... args) {
        if constexpr (std::is_same_v<Dir, undirected> && is_associative<EC>::value) {
            if (get_id_key(u) > get_id_key(v)) {
                std::swap(u, v);
            }
        }
        return push(edges_, edge_data(edge_vertices(u, v), std::forward<Ts>(args)...));
    }

    auto remove(edge_id id) { erase(edges_, id); }
};

template <class Dir, class EC, class VID>
class edge_list<Dir, EC, VID, none> {
public:
    // Edge property is duplicated if graph is undirected or bidirectional, so it's immutable.
    using out_edge_bundle =
        std::conditional_t<std::is_same_v<Dir, directed>, edge_property, const edge_property>;
};

}  // namespace ac::detail

namespace std {

template <class Dir, class VID>
struct hash<ac::detail::edge_vertices<Dir, ac::two_vertices, VID>> {
    auto operator()(const ac::detail::edge_vertices<Dir, ac::two_vertices, VID>& src) const {
        auto key = src.key();
        return ac::hash_combine(key.first, key.second);
    }
};

}  // namespace std
