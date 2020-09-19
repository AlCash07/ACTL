/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar/arithmetic.hpp>
#include <actl/functional/scalar/comparison.hpp>
#include <actl/functional/scalar/math.hpp>

namespace ac::math {

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E {
    struct is_policy;
};

template <class E>
inline constexpr auto apply_policy(Equal, const absolute_error<E>& policy) {
    return abs(sub) <= policy.epsilon();
}

template <class E>
inline constexpr auto apply_policy(Less, const absolute_error<E>& policy) {
    return policy.epsilon() < rhs_ - lhs_;
}

}  // namespace ac::math
