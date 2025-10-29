// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant.hpp>

namespace ac {

/// If all the types are the same excluding integral constants, provides the
/// member typedef type of this type. If all the types are integral constants,
/// the first of them is provided.
template<typename... Ts>
struct strict_common_type;

template<typename... Ts>
using strict_common_type_t =
    typename strict_common_type<std::decay_t<Ts>...>::type;

template<typename T>
struct strict_common_type<T> {
    using type = T;
};

template<typename T, typename U>
struct strict_common_type<T, U> {
    static auto f() {
        if constexpr (is_constant_v<U>)
            return T{};
        else if constexpr (is_constant_v<T>)
            return U{};
        else {
            static_assert(std::is_same_v<T, U>);
            return T{};
        }
    }

    using type = decltype(f());
};

template<typename T, typename U, typename... Ts>
struct strict_common_type<T, U, Ts...>
    : strict_common_type<strict_common_type_t<T, U>, Ts...> {};

} // namespace ac
