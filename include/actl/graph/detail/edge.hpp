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
    using id_type = std::conditional_t<CompareSrc, std::pair<V, E>, E>;

    explicit constexpr edge() = default;

    explicit constexpr edge(V u, V v, E e) : u_(u), v_(v), e_(e) {}

    constexpr V source() const { return u_; }
    constexpr V target() const { return v_; }

    constexpr id_type id() const {
        if constexpr (CompareSrc) {
            return std::make_pair(u_, e_);
        }
        return e_;
    }

    constexpr operator id_type() const { return id(); }

    bool operator < (const edge& rhs) const { return id() < rhs.id(); }
    bool operator == (const edge& rhs) const { return id() == rhs.id(); }
    bool operator != (const edge& rhs) const { return id() != rhs.id(); }

private:
    V u_;
    V v_;
    E e_;
};

}  // namespace ac::detail

namespace std {

template <class V, class E, bool CS>
struct hash<ac::detail::edge<V, E, CS>> : hash<typename ac::detail::edge<V, E, CS>::id_type> {};

}  // namespace std
