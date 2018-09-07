/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/hash.hpp>

namespace ac::detail {

template <class V, class E, bool CompareSrc = false>
class edge {
public:
    explicit edge() = default;

    explicit edge(V u, V v, E e) : u_(u), v_(v), e_(e) {}

    V source() const { return u_; }
    V target() const { return v_; }

    constexpr operator E() const { return e_; }

    bool operator < (const edge& other) const {
        if constexpr (CompareSrc) {
            if (u_ != other.u_) return u_ < other.u_;
        }
        return e_ < other.e_;
    }

    bool operator == (const edge& other) const {
        if constexpr (CompareSrc) {
            if (u_ != other.u_) return false;
        }
        return e_ == other.e_;
    }

    bool operator != (const edge& other) const { return !(operator == (other)); }

private:
    V u_;
    V v_;
    E e_;
};

}  // namespace ac::detail

namespace std {

template <class V, class E>
struct hash<ac::detail::edge<V, E, true>> {
    auto operator()(const ac::detail::edge<V, E, true>& edge) const {
        return ac::hash_value(edge.source(), (E)edge);
    }
};

template <class V, class E>
struct hash<ac::detail::edge<V, E, false>> : hash<E> {};

}  // namespace std
