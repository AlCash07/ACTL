/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/none.hpp>
#include <utility>

namespace ac::operators {

// Inherit from this class to enable ADL lookup to find operators in this namespace.
// Template base class enables empty base class chaining to avoid type size increasing,
// reference: https://www.boost.org/doc/libs/1_70_0/libs/utility/operators.htm#old_lib_note
template <class B = none>
struct base : B {
    using B::B;
};

// pass is the same as std::forward except it converts reference into const reference
template <class T>
inline T&& pass(std::remove_reference_t<T>& x) {
    return static_cast<T&&>(x);
}

template <class T>
inline const T&& pass(std::remove_reference_t<T>&& x) {
    static_assert(!std::is_lvalue_reference_v<T>, "can not pass an rvalue as an lvalue");
    return static_cast<const T&&>(x);
}

template <class T, class U>
inline constexpr auto operator != (T&& lhs, U&& rhs) -> decltype(!(pass<T>(lhs) == pass<U>(rhs))) {
    return !(pass<T>(lhs) == pass<U>(rhs));
}

template <class T, class U>
inline constexpr auto operator > (T&& lhs, U&& rhs) -> decltype(pass<U>(rhs) < pass<T>(lhs)) {
    return pass<U>(rhs) < pass<T>(lhs);
}

template <class T, class U>
inline constexpr auto operator <= (T&& lhs, U&& rhs) -> decltype(!(pass<T>(lhs) > pass<U>(rhs))) {
    return !(pass<T>(lhs) > pass<U>(rhs));
}

template <class T, class U>
inline constexpr auto operator >= (T&& lhs, U&& rhs) -> decltype(!(pass<T>(lhs) < pass<U>(rhs))) {
    return !(pass<T>(lhs) < pass<U>(rhs));
}

}  // namespace ac::operators
