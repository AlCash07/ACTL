/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation/operation.hpp>

namespace ac::math {

template <class OuterOp, class InnerOp>
struct composite_operation : operation<composite_operation<OuterOp, InnerOp>>, private InnerOp {
    using category = operation_tag;

    template <class... Ts>
    explicit constexpr composite_operation(Ts&&... xs) : InnerOp{std::forward<Ts>(xs)...} {}

    constexpr const InnerOp& inner() const { return static_cast<const InnerOp&>(*this); }

    using operation<composite_operation<OuterOp, InnerOp>>::operator();

    template <class... Ts>
    constexpr auto evaluate(const Ts&... xs) const {
        return OuterOp::evaluate(inner(), xs...);
    }
};

template <class OuterOp>
struct operation_composer {
    template <class InnerOp>
    constexpr auto operator()(InnerOp&& op) const {
        return composite_operation<OuterOp, remove_cvref_t<InnerOp>>{std::forward<InnerOp>(op)};
    }
};

template <class Outer, class Inner, class Policy>
inline constexpr auto apply_policy(const composite_operation<Outer, Inner>& op,
                                   const Policy& policy)
    -> decltype(operation_composer<Outer>{}(apply_policy(op.inner(), policy))) {
    return operation_composer<Outer>{}(apply_policy(op.inner(), policy));
}

}  // namespace ac::math
