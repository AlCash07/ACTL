// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/arithmetic/sub.hpp>
#include <actl/functional/scalar/common/arg.hpp>
#include <actl/functional/scalar/comparison/all.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E {
    struct is_policy;
};

template <class E>
constexpr auto apply_policy(equal_t, const absolute_error<E>& policy) {
    return abs(sub) <= policy.epsilon();
}

template <class E>
constexpr auto apply_policy(less_t, const absolute_error<E>& policy) {
    return policy.epsilon() < rhs_ - lhs_;
}

}  // namespace ac
