// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/math.hpp>
#include <actl/operation/scalar/arithmetic/sub.hpp>
#include <actl/operation/scalar/common/arg.hpp>
#include <actl/operation/scalar/comparison/all.hpp>

namespace ac {

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E
{
    struct is_policy;
};

template <class E>
constexpr auto apply_policy(scalar::equal_f, absolute_error<E> const& policy)
{
    return abs(sub) <= policy.epsilon();
}

template <class E>
constexpr auto apply_policy(scalar::less_f, absolute_error<E> const& policy)
{
    return policy.epsilon() < rhs_ - lhs_;
}

} // namespace ac
