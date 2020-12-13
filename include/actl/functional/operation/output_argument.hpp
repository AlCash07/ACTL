// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/argument_traits.hpp>

namespace ac::math {

template <bool In, class T>
struct out {
    T x;

    constexpr operator T() { return x; }

    template <class U>
    out& operator=(const U& y) {
        static_assert(!In, "use output instead of inplace argument for assignment");
        assign(*this, y);
        return *this;
    }
};

template <class T, bool NotIn = false>
struct is_out : std::false_type {};

template <bool In, class T, bool NotIn>
struct is_out<out<In, T>, NotIn> : std::bool_constant<!(NotIn && In)> {};

template <bool In, class T>
struct raw<out<In, T>> {
    using type = raw_t<T>;
};

template <class T>
inline constexpr const T& remove_inplace(const out<true, T>& x) {
    return x.x;
}

template <class T>
inline constexpr const T& remove_inplace(const T& x) {
    return x;
}

template <class T, class... Ts>
inline constexpr auto& find_dst(T&, Ts&... xs) {
    return find_dst(xs...);
}

template <class T, class... Ts>
inline constexpr T& find_dst(out<true, T>& x, Ts&... xs) {
    return x.x;
}

}  // namespace ac::math

namespace ac {

template <class T>
inline math::out<false, T> out(T&& x) {
    return {std::forward<T>(x)};
}

template <class T>
inline math::out<true, T> inplace(T&& x) {
    return {std::forward<T>(x)};
}

}  // namespace ac
