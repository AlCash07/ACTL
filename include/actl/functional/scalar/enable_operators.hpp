// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/policy.hpp>
#include <actl/traits/type_traits.hpp>

namespace ac {

namespace detail {

template <class T, class = void>
struct enable_operators_impl : std::false_type {};

template <class T>
struct enable_operators_impl<T, std::void_t<typename T::enable_operators>> : std::true_type {};

}  // namespace detail

template <class... Ts>
using enable_operators =
    std::enable_if_t<(... || detail::enable_operators_impl<remove_cvref_t<Ts>>::value) &&
                         !(... || is_policy_v<remove_cvref_t<Ts>>),
                     int>;

}  // namespace ac
