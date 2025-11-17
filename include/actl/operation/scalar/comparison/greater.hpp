// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/common/arg.hpp>
#include <actl/operation/scalar/comparison/less.hpp>

namespace ac {

struct greater_f : operation_base<greater_f> {
    using operation_category = ordering_operation_tag;

    static constexpr auto formula = r_ < l_;
};
inline constexpr greater_f greater;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator>(L&& l, R&& r) {
    return greater(pass<L>(l), pass<R>(r));
}

} // namespace ac
