// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/arithmetic_operation.hpp>
#include <cmath>

namespace ac {

struct SquareRoot : operation_base<SquareRoot> {
    using operation_category = exponential_operation;

    template<typename T>
    static constexpr auto evaluate(T x) {
        using std::sqrt;
        return sqrt(x);
    }
};
inline constexpr SquareRoot square_root;

} // namespace ac
