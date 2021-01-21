// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct copy_t : scalar_operation<copy_t, 1> {
    using category = scalar_operation_tag;
    using argument_category = arithmetic_tag;

    template <class T>
    static constexpr T eval_scalar(T x) {
        return x;
    }
};
constexpr copy_t copy;

template <class T, class U>
constexpr void assign(out_t<T>& dst, const U& y) {
    copy(dst, y);
}

}  // namespace ac
