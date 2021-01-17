// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct bit_not_t : scalar_operation<bit_not_t, 2, integral_tag> {
    template <class T>
    static constexpr T eval_scalar(T x) {
        return ~x;
    }
};
constexpr bit_not_t bit_not;

template <class T, enable_operators<T> = 0>
constexpr auto operator~(T&& x) {
    return bit_not(pass<T>(x));
}

}  // namespace ac
