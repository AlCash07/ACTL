// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/operation/Operation.hpp>
#include <cstddef>

namespace ac {

template<Operation Op, size_t ArgumentIndex>
struct is_argument_operation : std::false_type {};

template<Operation Op, size_t ArgumentIndex>
    requires requires { Op::is_argument_operation(ArgumentIndex); }
struct is_argument_operation<Op, ArgumentIndex> {
    static constexpr bool value = Op::is_argument_operation(ArgumentIndex);
};

} // namespace ac
