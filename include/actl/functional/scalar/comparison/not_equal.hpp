// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/comparison/equal.hpp>
#include <actl/functional/scalar/logical/logical_not.hpp>

namespace ac {

struct non_equal_t : comparison_operation<non_equal_t> {
    struct is_commutative;

    static constexpr auto formula = !equal;
};
constexpr non_equal_t not_equal;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator!=(T&& lhs, U&& rhs) {
    return not_equal(pass<T>(lhs), pass<U>(rhs));
}

}  // namespace ac
