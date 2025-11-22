// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/common/select.hpp>
#include <actl/operation/scalar/comparison/less.hpp>
#include <limits>

namespace ac {

struct Min : operation_base<Min> {
    using operation_category = scalar_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr auto evaluate(L l, R r) {
        return eval(select(less(r, l), r, l));
    }
};
inline constexpr Min min;

template<typename T>
struct identity_element<Min, T> {
    static constexpr T value() {
        return std::numeric_limits<T>::max();
    }
};

} // namespace ac
