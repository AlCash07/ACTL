// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/argument_traits.hpp>

namespace ac {

template <class T>
struct inout_t {
    struct enable_operators;

    T x;
};

template <class T>
struct is_inout : std::false_type {};

template <class T>
struct is_inout<inout_t<T>> : std::true_type {};

template <class T>
constexpr bool is_inout_v = is_inout<remove_cvref_t<T>>::value;

template <class... Ts>
constexpr bool is_any_inout_v = (... || is_inout_v<Ts>);

template <class T>
decltype(auto) inout(T&& x) {
    if constexpr (is_inout_v<T>)
        return std::forward<T>(x);
    else
        return inout_t<T>{std::forward<T>(x)};
}

template <class T>
struct raw<inout_t<T>> : raw<T> {};

template <class T>
constexpr const T& remove_inout(const inout_t<T>& x) {
    return x.x;
}

template <class T>
constexpr const T& remove_inout(const T& x) {
    return x;
}

template <class T, class... Ts>
constexpr auto& find_dst(T&, Ts&... xs) {
    return find_dst(xs...);
}

template <class T, class... Ts>
constexpr T& find_dst(inout_t<T>& x, Ts&...) {
    return x.x;
}

} // namespace ac
