// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct Copy : operation_base<Copy> {
    using operation_category = scalar_operation_tag;

    template<typename T>
    static constexpr T evaluate(T x) {
        return x;
    }
};
inline constexpr Copy copy;

} // namespace ac
