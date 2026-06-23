// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_not.hpp>

namespace ac {

struct BitNotScalar : operation_base<BitNotScalar> {
    using parent = BitNot::op;

    static constexpr auto evaluate(std::integral auto i) {
        return ~i;
    }
};
inline constexpr BitNotScalar bit_not_scalar;

namespace BitNot {
template<typename T>
    requires std::is_arithmetic_v<T>
constexpr auto specialization(op, type_array<T>) noexcept {
    return bit_not_scalar;
}
} // namespace BitNot

} // namespace ac
