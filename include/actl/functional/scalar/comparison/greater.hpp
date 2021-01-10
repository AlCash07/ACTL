// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/common/arg.hpp>
#include <actl/functional/scalar/comparison/less.hpp>

namespace ac {

struct greater_t : comparison_operation<greater_t> {
    static constexpr auto formula = rhs_ < lhs_;
};
constexpr greater_t greater;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator>(T&& lhs, U&& rhs) {
    return greater(pass<T>(lhs), pass<U>(rhs));
}

}  // namespace ac
