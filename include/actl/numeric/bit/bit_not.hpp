// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_not_scalar.hpp>

namespace ac {

struct BitNot : operation_base<BitNot> {
    using operation_category = bitwise_operation;

    template<typename T>
        requires std::is_arithmetic_v<T>
    friend constexpr auto specialization(BitNot, type_array<T>) noexcept {
        return bit_not_scalar;
    }
};
inline constexpr BitNot bit_not;

template<typename T>
    requires EnableOperators<T>
constexpr auto operator~(T&& t) {
    return bit_not(std::forward<T>(t));
}

} // namespace ac
