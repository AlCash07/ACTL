// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/operation/evaluation/is_argument_operation.hpp>
#include <actl/operation/operation/operation_base.hpp>

struct Empty : ac::operation_base<Empty> {};

static_assert(!ac::is_argument_operation<Empty, 0>::value);
static_assert(!ac::is_argument_operation<Empty, 1>::value);

struct Specifies : ac::operation_base<Specifies> {
    static constexpr bool is_argument_operation(size_t index) {
        return index == 1;
    }
};

static_assert(!ac::is_argument_operation<Specifies, 0>::value);
static_assert(ac::is_argument_operation<Specifies, 1>::value);
