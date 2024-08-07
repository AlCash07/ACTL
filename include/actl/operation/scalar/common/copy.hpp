// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct copy_f : scalar_operation<copy_f, 1> {
    using operation_category = scalar_operation_tag;

    template<class T>
    static constexpr T eval_scalar(T x) {
        return x;
    }
};
inline constexpr copy_f copy;

} // namespace ac
