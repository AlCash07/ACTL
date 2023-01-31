// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/policy/policy.hpp>

namespace ac {

template <class... Ts>
concept EnableOperators =
    (... || (requires {
                 typename std::remove_cvref_t<Ts>::enable_operators;
             })) && !(... || Policy<Ts>);

} // namespace ac
