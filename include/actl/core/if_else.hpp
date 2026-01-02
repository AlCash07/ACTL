// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/scalar_operation.hpp>

namespace ac {

struct IfElse : operation_base<IfElse> {
    using operation_category = scalar_operation;

    static constexpr bool is_argument_maybe_unused(size_t index) noexcept {
        return index == 1 || index == 2;
    }

    template<typename OnTrue, typename OnFalse>
    static constexpr auto evaluate(
        bool condition, OnTrue const& on_true, OnFalse const& on_false
    ) {
        return condition ? eval(on_true) : eval(on_false);
    }
};
inline constexpr IfElse if_else;

} // namespace ac
