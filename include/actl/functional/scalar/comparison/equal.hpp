// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/comparison/comparison_operation.hpp>

namespace ac {

struct equal_t : comparison_operation<equal_t> {
    struct is_commutative;

    template <class T, class U>
    static constexpr bool eval_scalar(T lhs, U rhs) {
        return lhs == rhs;
    }
};
constexpr equal_t equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator==(T&& lhs, U&& rhs) {
    return equal(pass<T>(lhs), pass<U>(rhs));
}

}  // namespace ac
