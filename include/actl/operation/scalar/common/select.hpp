// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct select_f : scalar_operation<select_f, 3> {
    using operation_category = scalar_operation_tag;

    static constexpr bool is_argument_maybe_unused(size_t index) noexcept {
        return index == 1 || index == 2;
    }

    template<typename L, typename R>
    constexpr auto evaluate(bool condition, L const& l, R const& r) const {
        return condition ? eval(l) : eval(r);
    }
};
inline constexpr select_f select;

} // namespace ac
