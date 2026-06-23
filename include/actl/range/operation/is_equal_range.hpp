// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/comparison/equality_scalar.hpp>
#include <actl/operation/higher_order_function/all.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <algorithm>

namespace ac {

struct IsEqualRange : operation_base<IsEqualRange> {
    static constexpr bool evaluate(
        Range auto& l, Range auto& r, auto const& equal_element
    ) {
        return std::equal(
            ranges::begin(l),
            ranges::end(l),
            ranges::begin(r),
            ranges::end(r),
            equal_element
        );
    }
};

inline constexpr auto is_equal_range = IsEqualRange{}(
    l_,
    r_,
    operation_arg(is_equal(
        type_operation<range_reference, Arg<0, 2>>,
        type_operation<range_reference, Arg<1, 2>>
    ))
);

namespace IsEqual {
template<Range L, Range R>
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return is_equal_range;
}
} // namespace IsEqual

} // namespace ac
