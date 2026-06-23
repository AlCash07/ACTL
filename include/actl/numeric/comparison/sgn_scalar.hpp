// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/compare_3way_scalar.hpp>
#include <actl/numeric/comparison/sgn.hpp>
#include <actl/numeric/constant_literals.hpp>
#include <actl/operation/arg.hpp>

namespace ac {

struct SgnScalar : operation_base<SgnScalar> {
    using parent = Sgn::op;

    static constexpr auto formula = compare_3way(t_, 0_c);
};
inline constexpr SgnScalar sgn_scalar;

namespace Sgn {
template<typename T>
    requires std::is_arithmetic_v<T>
constexpr auto specialization(op, type_array<T>) noexcept {
    return sgn_scalar;
}
} // namespace Sgn

} // namespace ac
