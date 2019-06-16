/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/none.hpp>
#include <cstddef>

namespace ac::operators {

// Inherit from this class to enable ADL lookup to find operators in this namespace.
// Template base class enables empty base class chaining to avoid increasing type size,
// reference: https://www.boost.org/doc/libs/1_70_0/libs/utility/operators.htm#old_lib_note
template <class B = none>
struct base : B {
    using B::B;
};

template <class T>
inline constexpr T operator++(T& x, int) {
    T copy = x;
    ++x;
    return copy;
}

template <class T>
inline constexpr T operator--(T& x, int) {
    T copy = x;
    --x;
    return copy;
}

template <class T, class U>
inline constexpr auto operator != (const T& lhs, const U& rhs) {
    return !(lhs == rhs);
}

template <class T, class U>
inline constexpr auto operator > (const T& lhs, const U& rhs) {
    return rhs < lhs;
}

template <class T, class U>
inline constexpr auto operator <= (const T& lhs, const U& rhs) {
    return !(lhs > rhs);
}

template <class T, class U>
inline constexpr auto operator >= (const T& lhs, const U& rhs) {
    return !(lhs < rhs);
}

}  // namespace ac::operators
