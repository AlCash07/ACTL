// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_operation.hpp>

namespace ac {

struct BitNot : operation_base<BitNot> {
    using operation_category = bitwise_operation;
};
inline constexpr BitNot bit_not;

template<typename T>
    requires EnableOperators<T>
constexpr auto operator~(T&& x) {
    return bit_not(pass<T>(x));
}

struct BitNotScalar : operation_base<BitNotScalar> {
    using parent = BitNot;

    template<typename T>
    static constexpr bool requirement = std::is_arithmetic_v<T>;

    static constexpr auto evaluate(std::integral auto x) {
        return ~x;
    }
};
AC_REGISTER_OVERLOAD(BitNotScalar)
inline constexpr BitNotScalar bit_not_scalar;

} // namespace ac
