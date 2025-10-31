// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/parameter/inout.hpp>
#include <actl/operation/core/argument_traits.hpp>

namespace ac {

template<typename... Ts>
inline constexpr bool is_any_inout_v = (... || is_inout_v<Ts>);

template<typename T>
struct raw<inout<T>> : raw<T> {};

template<typename T>
constexpr T const& remove_inout(inout<T>& x) {
    return *x;
}

template<typename T>
constexpr T const& remove_inout(T const& x) {
    return x;
}

template<typename T, typename... Ts>
constexpr auto& find_target(T&, Ts&... xs) {
    return find_target(xs...);
}

template<typename T, typename... Ts>
constexpr T& find_target(inout<T>& x, Ts&...) {
    return *x;
}

} // namespace ac
