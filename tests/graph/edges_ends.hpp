// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/graph/traits.hpp>
#include <actl/std/vector.hpp>
#include <actl/util/type_traits.hpp>

template <bool Directed, class Edges>
inline auto get_ends(const Edges& es) {
    using V = vertex_t<value_t<Edges>>;
    std::vector<std::pair<V, V>> res;
    for (auto e : es) {
        V u = e.source();
        V v = e.target();
        if constexpr (!Directed) {
            if (v < u) std::swap(u, v);
        }
        res.emplace_back(u, v);
    }
    return res;
}

template <class Edges>
inline auto get_sources(const Edges& es) {
    std::vector<vertex_t<value_t<Edges>>> res;
    for (auto e : es) res.emplace_back(e.source());
    return res;
}

template <class Edges>
inline auto get_targets(const Edges& es) {
    std::vector<vertex_t<value_t<Edges>>> res;
    for (auto e : es) res.emplace_back(e.target());
    return res;
}
