/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/composite/composite_operation.hpp>
#include <actl/functional/scalar/common.hpp>

namespace ac::math {

template <class Op, class T>
struct cast_before {
    struct is_policy;
};

template <class Op, class T>
inline constexpr auto apply_policy(const Op& op, cast_before<Op, T>) {
    return op(cast<T>);
}

}  // namespace ac::math
