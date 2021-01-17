// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct neg_t : scalar_operation<neg_t, 2, arithmetic_tag> {
    template <class T>
    static constexpr auto eval_scalar(T x) {
        return -x;
    }
};
constexpr neg_t neg;

template <class T, enable_operators<T> = 0>
constexpr auto operator-(T&& x) {
    return neg(pass<T>(x));
}

}  // namespace ac
