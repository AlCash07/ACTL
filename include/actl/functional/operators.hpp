/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/none.hpp>
#include <type_traits>

namespace ac::operators {

// Inherit from this class to enable ADL lookup to find operators in this namespace.
// Template base class enables empty base class chaining to avoid type size increasing,
// reference: https://www.boost.org/doc/libs/1_70_0/libs/utility/operators.htm#old_lib_note
template <class B = none>
struct base : B {
    using B::B;
};

template <class T, class U>
inline constexpr auto operator != (const T& lhs, const U& rhs) -> decltype(!(lhs == rhs)) {
    return !(lhs == rhs);
}

template <class T, class U>
inline constexpr auto operator > (const T& lhs, const U& rhs) -> decltype(rhs < lhs) {
    return rhs < lhs;
}

template <class T, class U>
inline constexpr auto operator <= (const T& lhs, const U& rhs) -> decltype(!(lhs > rhs)) {
    return !(lhs > rhs);
}

template <class T, class U>
inline constexpr auto operator >= (const T& lhs, const U& rhs) -> decltype(!(lhs < rhs)) {
    return !(lhs < rhs);
}

}  // namespace ac::operators
