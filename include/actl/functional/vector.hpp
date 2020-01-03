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

template <class Policy, class R0, class R1, enable_int_if<is_range_v<R0> && is_range_v<R1>> = 0>
inline bool perform(Equal, const Policy& policy, const R0& lhs, const R1& rhs) {
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs),
                      equal(policy));
}

template <class Policy, class R0, class R1, enable_int_if<is_range_v<R0> && is_range_v<R1>> = 0>
inline bool perform(Less, const Policy& policy, const R0& lhs, const R1& rhs) {
    return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                        std::end(rhs), less(policy));
}

}  // namespace ac::op
