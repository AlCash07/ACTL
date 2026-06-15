// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/type_operation.hpp>
#include <actl/range/traits/associated_types.hpp>

namespace ac {

struct Fold : operation_base<Fold> {
    // TODO: implement.
};

template<Operation Op>
constexpr auto fold(Op&& op) {
    return Fold{}(
        x_,
        std::forward<Op>(
            type_operation<range_reference, Arg<0, 1>>,
            type_operation<range_reference, Arg<0, 1>>
        )
    );
}

} // namespace ac
