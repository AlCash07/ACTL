// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/lifetime/conversion/as.hpp>
#include <actl/numeric/arithmetic/additive/subtract_scalar.hpp>
#include <actl/numeric/comparison/compare_3way.hpp>
#include <actl/numeric/comparison/ordering_scalar.hpp>

namespace ac {

struct Compare3WayScalar : operation_base<Compare3WayScalar> {
    using parent = Compare3Way::op;

    template<typename Args>
    friend constexpr auto specialization(Compare3WayScalar, Args) noexcept {
        return as<int>(is_greater) - as<int>(is_less);
    }
};
inline constexpr Compare3WayScalar compare_3way_scalar;

namespace Compare3Way {
template<typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return compare_3way_scalar;
}
} // namespace Compare3Way

} // namespace ac
