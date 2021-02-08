// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/common/arg.hpp>
#include <actl/operation/scalar/comparison/less.hpp>

namespace ac {

struct greater_f : scalar_operation<greater_f, 2> {
    using category = ordering_operation_tag;
    using argument_category = scalar_tag;

    static constexpr auto formula = rhs_ < lhs_;
};
inline constexpr greater_f greater;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator>(T&& lhs, U&& rhs) {
    return greater(pass<T>(lhs), pass<U>(rhs));
}

} // namespace ac
