// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/std/utility.hpp>

namespace ac::detail {

template<typename V, typename B, bool CompareSrc = false>
class edge {
public:
    using vertex = V;
    using id_type = std::conditional_t<CompareSrc, std::pair<V, B>, B>;

    explicit constexpr edge() = default;

    explicit constexpr edge(V u, V v, B b) : m_u{u}, m_v{v}, m_b{b} {}

    constexpr V source() const {
        return m_u;
    }

    constexpr V target() const {
        return m_v;
    }

    constexpr B bundle() const {
        return m_b;
    }

    constexpr id_type id() const {
        if constexpr (CompareSrc) {
            return std::pair{m_u, m_b};
        } else {
            return m_b;
        }
    }

    constexpr operator id_type() const {
        return id();
    }

    bool operator<(edge const& rhs) const {
        return id() < rhs.id();
    }

    // TODO: for undirected and bidirectional graphs edge isn't equal to its
    // reverse.
    bool operator==(edge const& rhs) const {
        return id() == rhs.id();
    }

private:
    friend struct ac::hash_access;

    size_t hash() const {
        return hash_value(id());
    }

    V m_u;
    V m_v;
    B m_b;
};

} // namespace ac::detail
