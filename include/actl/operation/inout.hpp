// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/parameters/inout.hpp>
#include <actl/operation/core/argument_traits.hpp>

namespace ac {

template<class... Ts>
inline constexpr bool is_any_inout_v =
    (... || is_inout_v<std::remove_cvref_t<Ts>>);

template<class T>
struct raw<inout<T>> : raw<T> {};

template<class T>
constexpr T const& remove_inout(inout<T> const& x) {
    return x.x;
}

template<class T>
constexpr T const& remove_inout(T const& x) {
    return x;
}

template<class T, class... Ts>
constexpr auto& find_dst(T&, Ts&... xs) {
    return find_dst(xs...);
}

template<class T, class... Ts>
constexpr T& find_dst(inout<T>& x, Ts&...) {
    return x.x;
}

} // namespace ac
