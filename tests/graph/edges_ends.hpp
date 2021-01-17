// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/graph/traits.hpp>
#include <actl/meta/dependent.hpp>
#include <actl/std/vector.hpp>

template <bool Directed, class Edges>
auto get_ends(const Edges& es) {
    using V = ac::vertex_t<ac::value_type_t<Edges>>;
    std::vector<std::pair<V, V>> res;
    for (auto e : es) {
        V u = e.source();
        V v = e.target();
        if constexpr (!Directed) {
            if (v < u)
                std::swap(u, v);
        }
        res.emplace_back(u, v);
    }
    return res;
}

template <class Edges>
auto get_sources(const Edges& es) {
    std::vector<ac::vertex_t<ac::value_type_t<Edges>>> res;
    for (auto e : es)
        res.emplace_back(e.source());
    return res;
}

template <class Edges>
auto get_targets(const Edges& es) {
    std::vector<ac::vertex_t<ac::value_type_t<Edges>>> res;
    for (auto e : es)
        res.emplace_back(e.target());
    return res;
}
