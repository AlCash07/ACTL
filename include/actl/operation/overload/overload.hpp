// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

template<typename Op>
struct default_overload {};

template<typename Op>
    requires requires { Op::formula; }
struct default_overload<Op> {
    static constexpr auto formula = Op::formula;
};

template<typename Op, typename... Ts>
struct overload : default_overload<Op> {};

} // namespace ac
