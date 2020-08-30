/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar/comparison.hpp>
#include <actl/functional/scalar/derived.hpp>

namespace ac::math {

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E {
    struct is_policy;
};

template <class E, class T, enable_int_if<std::is_floating_point_v<T>> = 0>
inline constexpr int perform_policy(Sgn, const absolute_error<E>& policy, const T& x) {
    if (eval(less(abs(x), policy.epsilon()))) return 0;
    return x < 0 ? -1 : 1;
}

template <class Op, class E, class T, class U,
          enable_int_if<is_comparison_v<Op> &&
                        (std::is_floating_point_v<T> || std::is_floating_point_v<U>)> = 0>
inline constexpr auto perform_policy(Op op, const absolute_error<E>& policy, const T& lhs,
                                     const U& rhs) {
    return op(sgn(sub(lhs, rhs)), 0);
}

}  // namespace ac::math
