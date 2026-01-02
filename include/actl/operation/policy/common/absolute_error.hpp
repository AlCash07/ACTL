// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/subtract.hpp>
#include <actl/numeric/comparison/all.hpp>
#include <actl/operation/scalar/common/arg.hpp>

namespace ac {

// class E should provide public `T epsilon()`;
template<typename E>
struct absolute_error : E {
    struct is_policy;
};

template<typename E>
constexpr auto apply_policy(IsEqualScalar, absolute_error<E> const& policy) {
    return abs(subtract) <= policy.epsilon();
}

template<typename E>
constexpr auto apply_policy(IsLessScalar, absolute_error<E> const& policy) {
    return policy.epsilon() < r_ - l_;
}

} // namespace ac
