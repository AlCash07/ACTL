// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/equality.hpp>
#include <actl/operation/operation/composite_operation.hpp>
#include <algorithm>

namespace ac {

struct IsEqualRange {
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
inline constexpr operation_composer<IsEqualRange> is_equal_range;

template<Range L, Range R>
struct overload<IsEqual, L, R> {
    static constexpr auto formula =
        is_equal_range(resolve_overload<range_value_t<L>, range_value_t<R>>(
            default_context{}, is_equal
        ));
};

} // namespace ac
