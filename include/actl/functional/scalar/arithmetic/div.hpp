// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac {

struct div_t : scalar_operation<div_t, 2, arithmetic_tag> {
    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs / rhs;
    }
};
constexpr div_t div;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator/(T&& lhs, U&& rhs) {
    return div(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator/=(T&& lhs, U&& rhs) {
    return div(inout(std::forward<T>(lhs)), pass<U>(rhs));
}

}  // namespace ac