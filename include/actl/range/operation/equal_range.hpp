// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/range.hpp>
#include <actl/operation/core/composite_operation.hpp>
#include <actl/operation/scalar/comparison/equal.hpp>
#include <algorithm>

namespace ac {

struct equal_range_t {
    static constexpr index inner_count = 1;

    template <class EqualOp, class T, class U>
    static bool evaluate(const EqualOp& op, const T& lhs, const U& rhs) {
        return std::equal(
            std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), op);
    }
};
inline constexpr operation_composer<equal_range_t> equal_range;

template <class T, class U>
struct overload<equal_t, range_tag, T, U> {
    static constexpr auto resolve(equal_t op) {
        return equal_range(op.resolve_nested<T, U>());
    }
};

} // namespace ac
