/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar.hpp>
#include <actl/range/traits.hpp>
#include <algorithm>

namespace ac::math {

struct range_tag {
    struct has_nested;
};

template <class T>
// struct category_impl<T, std::enable_if_t<is_range_v<T> && !is_contiguous_range_v<T>>> {
struct category_impl<T, std::enable_if_t<is_range_v<T>>> {
    using type = range_tag;
};

template <class Op>
struct range_calculator;

template <>
struct range_calculator<Equal> {
    static auto can_eval(...) -> std::true_type;

    template <class Policy, class R0, class R1>
    static bool evaluate(const Policy& policy, const R0& lhs, const R1& rhs) {
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs),
                          equal(policy));
    }
};

template <>
struct range_calculator<Less> {
    static auto can_eval(...) -> std::true_type;

    template <class Policy, class R0, class R1>
    static bool evaluate(const Policy& policy, const R0& lhs, const R1& rhs) {
        return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                            std::end(rhs), less(policy));
    }
};

template <class Op, enable_int_if<is_comparison_v<Op>> = 0>
inline range_calculator<Op> get_calculator(Op, range_tag);

}  // namespace ac::math
