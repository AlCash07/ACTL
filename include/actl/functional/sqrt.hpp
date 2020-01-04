/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar.hpp>
#include <actl/numeric/math.hpp>

namespace ac::op {

/**
 * Comparable square root with deferred call to sqrt.
 */
template <class T>
class square_root {
public:
    constexpr explicit square_root(const T& value = {}) : sqr_{value} {}

    operator decltype(adl::sqrt(std::declval<T>()))() const { return adl::sqrt(sqr_); }

    friend constexpr const T& perform(policy, Sqr, const square_root& x) { return x.sqr_; }

private:
    T sqr_;
};

struct defer_sqrt : virtual policy {};

template <class T>
inline auto sqrt(policy, const T& x) {
    return adl::sqrt(x);
}

template <class T>
inline auto sqrt(defer_sqrt, const T& x) {
    return square_root{x};
}

template <class T, class U>
inline constexpr auto perform(Equal, const square_root<T>& lhs, const square_root<U>& rhs) {
    return equal(sqr(lhs), sqr(rhs));
}

template <class T, class U>
inline constexpr auto perform(Less, const square_root<T>& lhs, const square_root<U>& rhs) {
    return less(sqr(lhs), sqr(rhs));
}

}  // namespace ac::op
