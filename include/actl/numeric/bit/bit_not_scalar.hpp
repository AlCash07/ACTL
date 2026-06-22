// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_operation.hpp>

namespace ac {

struct BitNot;

struct BitNotScalar : operation_base<BitNotScalar> {
    using parent = BitNot;

    static constexpr auto evaluate(std::integral auto i) {
        return ~i;
    }
};
inline constexpr BitNotScalar bit_not_scalar;

} // namespace ac
