/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/none.hpp>
#include <actl/util/type_traits.hpp>

namespace ac {

namespace op {

// Policies must be defined in op namespace for ADL to work.
struct policy {};

// Inherit from this class to enable ADL lookup to find operators in this namespace.
// Template base class enables empty base class chaining to avoid type size increasing,
// reference: https://www.boost.org/doc/libs/1_70_0/libs/utility/operators.htm#old_lib_note
template <class B = none>
struct base : B {
    using B::B;
};

}  // namespace op

inline constexpr op::policy default_policy;

template <class T>
inline constexpr bool is_policy_v = std::is_base_of_v<op::policy, remove_cvref_t<T>>;

template <class T>
using enable_int_if_policy = enable_int_if<is_policy_v<T>>;

template <class T>
using disable_int_if_policy = enable_int_if<!is_policy_v<T>>;

#define DEFINE_HAS_OVERLOAD(name)                                                \
    template <class, class... Ts>                                                \
    struct has_##name : std::false_type {};                                      \
    template <class... Ts>                                                       \
    struct has_##name<std::void_t<decltype(name(std::declval<Ts>()...))>, Ts...> \
        : std::true_type {};                                                     \
    template <class... Ts>                                                       \
    inline constexpr bool has_##name##_v = has_##name<void, op::policy, Ts...>::value;

#define DEFINE_HAS_BINARY_OPERATOR(name, symbol)                                               \
    template <class T, class U, class = void>                                                  \
    struct has_##name : std::false_type {};                                                    \
    template <class T, class U>                                                                \
    struct has_##name<T, U, std::void_t<decltype(std::declval<T>() symbol std::declval<U>())>> \
        : std::true_type {};                                                                   \
    template <class T, class U>                                                                \
    inline constexpr bool has_##name##_v = has_##name<T, U>::value;

}  // namespace ac
