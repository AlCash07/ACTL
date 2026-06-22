// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/compare_3way_scalar.hpp>

namespace ac {

struct Compare3Way : operation_base<Compare3Way> {
    using operation_category = ordering_operation;

    template<typename L, typename R>
        requires(
            (is_constant_v<L> || std::is_arithmetic_v<L>) &&
            (is_constant_v<R> || std::is_arithmetic_v<R>)
        )
    friend constexpr auto specialization(
        Compare3Way, type_array<L, R>
    ) noexcept {
        return compare_3way_scalar;
    }
};
inline constexpr Compare3Way compare_3way;

} // namespace ac
