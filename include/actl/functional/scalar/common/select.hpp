// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac {

struct select_t : scalar_operation<select_t, 3, arithmetic_tag> {
    template <class T, class U>
    constexpr auto evaluate(bool condition, const T& lhs, const U& rhs) const {
        return condition ? eval(lhs) : eval(rhs);
    }
};
constexpr select_t select;

}  // namespace ac
