// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/parameter/inout.hpp>
#include <actl/operation/expression/raw.hpp>

namespace ac {

template<typename T>
struct raw<inout<T>> : raw<T> {};

template<typename T>
constexpr T const& remove_inout(inout<T>& t) {
    return *t;
}

template<typename T>
constexpr T const& remove_inout(T const& t) {
    return t;
}

template<typename T, typename... Ts>
constexpr auto& find_target(T&, Ts&... ts) {
    return find_target(ts...);
}

template<typename T, typename... Ts>
constexpr T& find_target(inout<T>& t, Ts&...) {
    return *t;
}

} // namespace ac
