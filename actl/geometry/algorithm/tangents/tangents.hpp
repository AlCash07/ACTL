/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits/id.hpp>
#include <actl/type/use_default.hpp>

namespace ac {

template <class Policy, class T0, class T1, class OutputIterator,
          class = geometry::enable_if_swap_t<T0, T1>>
inline auto tangents(const Policy& policy, const T0& lhs, const T1& rhs, OutputIterator dst) {
    return tangents(policy, rhs, lhs, dst);
}

template <class T0, class T1, class OutputIterator>
inline auto tangents(const T0& lhs, const T1& rhs, OutputIterator dst) {
    return tangents(use_default(), lhs, rhs, dst);
}

}  // namespace ac
