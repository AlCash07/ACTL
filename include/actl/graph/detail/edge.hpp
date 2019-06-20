/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/std/utility.hpp>

namespace ac::detail {

template <class V, class B, bool CompareSrc = false>
class edge {
public:
    using vertex  = V;
    using id_type = std::conditional_t<CompareSrc, std::pair<V, B>, B>;

    explicit constexpr edge() = default;
    explicit constexpr edge(V u, V v, B b) : u_{u}, v_{v}, b_{b} {}

    constexpr V source() const { return u_; }
    constexpr V target() const { return v_; }
    constexpr B bundle() const { return b_; }

    constexpr id_type id() const {
        if constexpr (CompareSrc) {
            return std::pair{u_, b_};
        } else {
            return b_;
        }
    }

    constexpr operator id_type() const { return id(); }

    bool operator < (const edge& rhs) const { return id() < rhs.id(); }
    // TODO: for undirected and bidirectional graphs edge isn't equal to its reverse.
    bool operator == (const edge& rhs) const { return id() == rhs.id(); }
    bool operator != (const edge& rhs) const { return id() != rhs.id(); }

private:
    friend struct ac::hash_access;

    size_t hash() const { return hash_value(id()); }

    V u_;
    V v_;
    B b_;
};

}  // namespace ac::detail
