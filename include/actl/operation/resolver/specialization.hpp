// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/Operation.hpp>
#include <actl/sequence/type_array/type_array.hpp>

namespace ac {

template<Operation Op, typename ArgsArray>
    requires requires { Op::formula; }
constexpr auto specialization(Op, ArgsArray) noexcept {
    return Op::formula;
}

} // namespace ac
