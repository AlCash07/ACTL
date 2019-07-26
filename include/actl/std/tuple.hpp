/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <tuple>

namespace ac {

template <class T, size_t... Is>
inline constexpr size_t hash_tuple(const T& x, std::index_sequence<Is...>) {
    return hash_value(std::get<Is>(x)...);
}

template <class... Ts>
inline constexpr size_t hash_value(const std::tuple<Ts...>& x) {
    return hash_tuple(x, std::make_index_sequence<sizeof...(Ts)>{});
}

namespace op {

template <size_t I = 0, class Policy, class... Ts, class... Us>
inline bool equal(Policy&& policy, const std::tuple<Ts...>& lhs, const std::tuple<Us...>& rhs) {
    if constexpr (I == sizeof...(Ts)) {
        return true;
    } else {
        return equal(policy, std::get<I>(lhs), std::get<I>(rhs)) && equal<I + 1>(policy, lhs, rhs);
    }
}

template <size_t I = 0, class Policy, class... Ts, class... Us>
inline bool less(Policy&& policy, const std::tuple<Ts...>& lhs, const std::tuple<Us...>& rhs) {
    if constexpr (I == sizeof...(Ts)) {
        return false;
    } else {
        int v = sgn(policy, std::get<I>(lhs), std::get<I>(rhs));
        return v < 0 || (v == 0 && less<I + 1>(policy, lhs, rhs));
    }
}

}  // namespace op

namespace io {

template <class... Ts>
struct is_composite<std::tuple<Ts...>> : std::true_type {};

namespace detail {

template <class D, class F, class T, size_t... Is>
inline index write_tuple(D& od, F& fmt, const T& x, std::index_sequence<Is...>) {
    return write(od, fmt, std::get<Is>(x)...);
}

template <class D, class F, class T, size_t... Is>
inline bool read_tuple(D& id, F& fmt, T& x, std::index_sequence<Is...>) {
    return read(id, fmt, std::get<Is>(x)...);
}

}  // namespace detail

template <class Device, class Format, class... Ts>
inline index serialize(Device& od, Format& fmt, const std::tuple<Ts...>& x) {
    return detail::write_tuple(od, fmt, x, std::make_index_sequence<sizeof...(Ts)>{});
}

template <class Device, class Format, class... Ts>
inline bool deserialize(Device& id, Format& fmt, std::tuple<Ts...>& x) {
    return detail::read_tuple(id, fmt, x, std::make_index_sequence<sizeof...(Ts)>{});
}

}  // namespace io

}  // namespace ac
