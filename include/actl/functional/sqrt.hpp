/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/policy.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

/**
 * Comparable square root with deferred call to sqrt.
 */
template <class T>
class square_root {
public:
    constexpr explicit square_root(const T& value) : sqr_{value} {}

    operator decltype(adl::sqrt(std::declval<T>()))() const { return adl::sqrt(sqr_); }

    friend constexpr const T& sqr(const square_root& x) { return x.sqr_; }

private:
    T sqr_;
};

namespace op {

struct defer_sqrt : virtual policy {};

template <class T>
inline auto sqrt(policy, const T& x) {
    return adl::sqrt(x);
}

template <class T>
inline auto sqrt(defer_sqrt, const T& x) {
    return square_root{x};
}

template <class Policy, class T, class U>
inline constexpr bool equal(const Policy& policy, const square_root<T>& lhs,
                            const square_root<U>& rhs) {
    return equal(policy, sqr(lhs), sqr(rhs));
}

template <class Policy, class T, class U>
inline constexpr bool less(const Policy& policy, const square_root<T>& lhs,
                           const square_root<U>& rhs) {
    return less(policy, sqr(lhs), sqr(rhs));
}

}  // namespace op

}  // namespace ac
