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

    template<typename EqualOp, typename L, typename R>
    static bool evaluate(EqualOp const& op, L const& l, R const& r) {
        return std::equal(
            ranges::begin(l),
            ranges::end(l),
            ranges::begin(r),
            ranges::end(r),
            op
        );
    }
};
inline constexpr operation_composer<equal_range_f> equal_range;

template<Range L, Range R>
struct overload<equal_f, L, R> {
    static constexpr auto formula = equal_range(resolve_nested<L, R>(equal));
};

} // namespace ac
