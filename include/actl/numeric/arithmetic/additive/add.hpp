// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/additive/add_scalar.hpp>

namespace ac {

struct Add : operation_base<Add> {
    using operation_category = additive_operation;

    static constexpr auto identity_element = 0_c;

    template<typename L, typename R>
        requires(std::integral<L> && std::integral<R>)
    friend constexpr auto specialization(Add, type_array<L, R>) noexcept {
        return add_integer;
    }

    template<typename L, typename R>
        requires(std::floating_point<L> && std::floating_point<R>)
    friend constexpr auto specialization(Add, type_array<L, R>) noexcept {
        return add_float;
    }
};
inline constexpr Add add;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator+(L&& l, R&& r) {
    return add(std::forward<L>(l), std::forward<R>(r));
}

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr decltype(auto) operator+=(L&& l, R&& r) {
    return add(inout{std::forward<L>(l)}, std::forward<R>(r));
}

} // namespace ac
