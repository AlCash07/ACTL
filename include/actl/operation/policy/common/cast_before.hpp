// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/as.hpp>
#include <actl/operation/operation/composite_operation.hpp>

namespace ac {

template<Operation Op, typename T>
struct cast_before {
    struct is_policy;
};

template<Operation Op, typename T>
constexpr auto apply_policy(Op const& op, cast_before<Op, T>) {
    return op(as<T>);
}

} // namespace ac
