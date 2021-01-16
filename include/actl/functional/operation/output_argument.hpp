// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/argument_traits.hpp>

namespace ac {

template <bool In, class T>
struct out_t {
    struct enable_operators;

    T x;

    constexpr operator T() {
        return x;
    }

    template <class U>
    out_t& operator=(const U& y) {
        static_assert(!In, "use out instead of inout argument for assignment");
        assign(*this, y);
        return *this;
    }
};

template <class T, bool NotIn = false>
struct is_out : std::false_type {};

template <bool In, class T, bool NotIn>
struct is_out<out_t<In, T>, NotIn> : std::bool_constant<!(NotIn && In)> {};

template <bool In, class T>
struct raw<out_t<In, T>> {
    using type = raw_t<T>;
};

template <class T>
constexpr const T& remove_inout(const out_t<true, T>& x) {
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
constexpr T& find_dst(out_t<true, T>& x, Ts&...) {
    return x.x;
}

template <class T>
out_t<false, T> out(T&& x) {
    return {std::forward<T>(x)};
}

template <class T>
out_t<true, T> inout(T&& x) {
    return {std::forward<T>(x)};
}

}  // namespace ac
