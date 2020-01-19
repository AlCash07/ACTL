/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/math.hpp>
#include <actl/functional/scalar.hpp>
#include <actl/range/traits.hpp>

namespace ac::op {

struct allow_promotion : virtual policy {};

template <class, class... Ts>
struct can_promote : std::false_type {};

template <class... Ts>
struct can_promote<std::void_t<std::common_type_t<Ts...>>, Ts...> {
    using T = std::common_type_t<Ts...>;

    static constexpr bool value = !is_range_v<T> && (... || !std::is_same_v<T, Ts>);
};

template <class Op, class... Ts,
          enable_int_if<is_scalar_operation_v<Op> && can_promote<void, Ts...>::value> = 0>
inline auto perform(allow_promotion, Op op, const Ts&... xs) {
    return op(static_cast<std::common_type_t<Ts...>>(xs)...);
}

template <class Op, class T>
struct cast_before : virtual policy {};

template <class T, class Op, class... Ts>
inline constexpr auto perform(cast_before<Op, T>, Op op, const Ts&... xs)
    -> decltype(perform(default_policy, op, static_cast<T>(xs)...)) {
    return perform(default_policy, op, static_cast<T>(xs)...);
}

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E, virtual policy {};

template <class E, class T, enable_int_if<std::is_floating_point_v<T>> = 0>
inline constexpr int perform(const absolute_error<E>& policy, Sgn, const T& x) {
    if (less(abs(x), policy.epsilon())) return 0;
    return x < 0 ? -1 : 1;
}

template <class Op, class E, class T, class U,
          enable_int_if<is_comparison_operation_v<Op> &&
                        (std::is_floating_point_v<T> || std::is_floating_point_v<U>)> = 0>
inline constexpr auto perform(const absolute_error<E>& policy, Op op, const T& lhs, const U& rhs) {
    return op(sgn(sub(lhs, rhs)), 0);
}

}  // namespace ac::op
