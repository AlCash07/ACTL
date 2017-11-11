/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functions.hpp>
#include <actl/math.hpp>

namespace ac {

/**
 * Comparable square root with deferred call to sqrt.
 */
template <class T>
class square_root {
public:
    constexpr square_root() = default;
    constexpr square_root(const T& value) : sqr_{sqr(value)} {}

    template <class T1>
    explicit operator T1 () const {
        return math::sqrt(static_cast<T1>(sqr_));
    }

    template <class T1>
    friend constexpr square_root<T1> deferred_sqrt(const T1& value);

    friend constexpr T sqr(const square_root& x) { return x.sqr_; }

private:
    constexpr square_root(const T& value, int) : sqr_{value} {}

    T sqr_;

#define SQUARE_ROOT_COMPARE(v)                                                                  \
    template <class T0, class T1>                                                               \
    friend constexpr bool operator v (const square_root<T0>& lhs, const square_root<T1>& rhs) { \
        return sqr(lhs) v sqr(rhs);                                                             \
    }

    SQUARE_ROOT_COMPARE(==)
    SQUARE_ROOT_COMPARE(!=)
    SQUARE_ROOT_COMPARE(<)
    SQUARE_ROOT_COMPARE(>)
    SQUARE_ROOT_COMPARE(<=)
    SQUARE_ROOT_COMPARE(>=)

#undef SQUARE_ROOT_COMPARE

    friend constexpr int sgn(const square_root& lhs, const square_root& rhs) {
        return sgn(sqr(lhs), sqr(rhs));
    }

    template <class T0, class T1>
    friend constexpr auto operator * (const square_root<T0>& lhs, const square_root<T1>& rhs) {
        return deferred_sqrt(sqr(lhs) * sqr(rhs));
    }
};

template <class T>
inline constexpr square_root<T> deferred_sqrt(const T& value) {
    return square_root<T>(value, 0);
}

}  // namespace ac
