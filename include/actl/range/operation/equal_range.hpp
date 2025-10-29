// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/composite_operation.hpp>
#include <actl/operation/scalar/comparison/equal.hpp>
#include <actl/range/traits/nesting_depth.hpp>
#include <algorithm>

namespace ac {

struct equal_range_f {
    static constexpr size_t inner_count = 1;

    template<typename EqualOp, typename T, typename U>
    static bool evaluate(EqualOp const& op, T const& lhs, U const& rhs) {
        return std::equal(
            ranges::begin(lhs),
            ranges::end(lhs),
            ranges::begin(rhs),
            ranges::end(rhs),
            op
        );
    }
};
inline constexpr operation_composer<equal_range_f> equal_range;

template<Range T, Range U>
struct overload<equal_f, T, U> {
    static constexpr auto formula = equal_range(resolve_nested<T, U>(equal));
};

} // namespace ac
