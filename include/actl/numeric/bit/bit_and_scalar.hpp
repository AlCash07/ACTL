// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_and.hpp>

namespace ac {

struct BitAndScalar : operation_base<BitAndScalar> {
    using parent = BitAnd::op;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<std::integral L, std::integral R>
    static constexpr auto evaluate(L l, R r) {
        return l & r;
    }
};
inline constexpr BitAndScalar bit_and_scalar;

namespace BitAnd {
template<typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return bit_and_scalar;
}
} // namespace BitAnd

} // namespace ac
