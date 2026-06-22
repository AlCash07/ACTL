// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_operation.hpp>

namespace ac {

struct BitXor;

struct BitXorScalar : operation_base<BitXorScalar> {
    using parent = BitXor;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<std::integral L, std::integral R>
    static constexpr auto evaluate(L l, R r) {
        return l ^ r;
    }
};
inline constexpr BitXorScalar bit_xor_scalar;

} // namespace ac
