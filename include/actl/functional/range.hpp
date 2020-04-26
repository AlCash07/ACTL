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

namespace ac::op {

struct range_tag {
    struct has_nested;
};

template <class T>
struct category_impl<T, std::enable_if_t<is_range_v<T>>> {
    using type = range_tag;
};

template <>
struct calculator<range_tag, Equal> {
    static auto can_eval(...) -> std::true_type;

    template <class Policy, class R0, class R1>
    static bool evaluate(const Policy& policy, const R0& lhs, const R1& rhs) {
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs),
                          equal(policy));
    }
};

template <>
struct calculator<range_tag, Less> {
    static auto can_eval(...) -> std::true_type;

    template <class Policy, class R0, class R1>
    static bool evaluate(const Policy& policy, const R0& lhs, const R1& rhs) {
        return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                            std::end(rhs), less(policy));
    }
};

}  // namespace ac::op
