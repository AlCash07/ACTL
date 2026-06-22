// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_operation.hpp>

namespace ac {

struct BitOr;

struct BitOrScalar : operation_base<BitOrScalar> {
    using parent = BitOr;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<std::integral L, std::integral R>
    static constexpr auto evaluate(L l, R r) {
        return l | r;
    }
};
inline constexpr BitOrScalar bit_or_scalar;

} // namespace ac
