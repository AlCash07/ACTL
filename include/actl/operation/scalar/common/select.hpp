// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct select_f : scalar_operation<select_f, 3>
{
    using operation_category = scalar_operation_tag;

    template <class T, class U>
    constexpr auto evaluate(bool condition, T const& lhs, U const& rhs) const
    {
        return condition ? eval(lhs) : eval(rhs);
    }
};
inline constexpr select_f select;

} // namespace ac
