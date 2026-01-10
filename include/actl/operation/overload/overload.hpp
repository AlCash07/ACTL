// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/Operation.hpp>
#include <actl/sequence/type_array/type_array.hpp>

namespace ac {

template<Operation Op>
struct default_overload {};

template<Operation Op>
    requires requires { Op::formula; }
struct default_overload<Op> {
    static constexpr auto formula = Op::formula;
};

template<Operation Op, typename... Args>
struct overload : default_overload<Op> {};

#define AC_REGISTER_OVERLOAD(name)                    \
    template<typename... Args>                        \
        requires name::requirement<Args...>           \
    struct overload<typename name::parent, Args...> { \
        static constexpr auto formula = name{};       \
    };

} // namespace ac
