// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct logical_not_t : scalar_operation<logical_not_t, 1, boolean_tag> {
    static constexpr bool eval_scalar(bool x) {
        return !x;
    }
};
constexpr logical_not_t logical_not;

template <class T, enable_operators<T> = 0>
constexpr auto operator!(T&& x) {
    return logical_not(pass<T>(x));
}

}  // namespace ac
