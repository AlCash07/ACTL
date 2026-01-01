// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_not.hpp>
#include <actl/operation/scalar/comparison/greater.hpp>

namespace ac {

struct LessEqual : operation_base<LessEqual> {
    using operation_category = ordering_operation_tag;

    static constexpr auto formula = !greater;
};
inline constexpr LessEqual less_equal;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator<=(L&& l, R&& r) {
    return less_equal(pass<L>(l), pass<R>(r));
}

} // namespace ac
