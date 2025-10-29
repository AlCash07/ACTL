// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/policy/policy.hpp>

namespace ac {

template<typename Op, typename Policy>
struct tuned_operation : operation<tuned_operation<Op, Policy>> {
    // [[no_unique_address]]
    Op operation;
    Policy policy;
};

template<typename Op, Policy P>
    requires Operation<std::remove_cvref_t<Op>>
constexpr auto operator|(Op&& op, P&& policy) {
    return tuned_operation<value_if_small<Op>, value_if_small<P>>{
        {}, std::forward<Op>(op), std::forward<P>(policy)
    };
}

} // namespace ac
