// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

template <class To>
struct cast_t : scalar_operation<cast_t<To>, 1> {
    using category = scalar_operation_tag;
    using argument_category = arithmetic_tag;

    template <class T>
    static constexpr To eval_scalar(T x) {
        return static_cast<To>(x);
    }
};
template <class T>
constexpr cast_t<T> cast;

}  // namespace ac
