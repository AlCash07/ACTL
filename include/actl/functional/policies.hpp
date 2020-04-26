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

namespace ac::math {

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
inline auto perform_policy(Op op, allow_promotion, const Ts&... xs) {
    return op(cast<std::common_type_t<Ts...>>(xs)...);
}

template <class Op, class T>
struct cast_before : virtual policy {};

template <class Op, class T, class... Ts>
inline constexpr auto perform_policy(Op op, cast_before<Op, T>, const Ts&... xs)
    -> decltype(perform_policy(op, default_policy, static_cast<T>(xs)...)) {
    return perform_policy(op, default_policy, static_cast<T>(xs)...);
}

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E, virtual policy {};

template <class E, class T, enable_int_if<std::is_floating_point_v<T>> = 0>
inline constexpr int perform_policy(Sgn, const absolute_error<E>& policy, const T& x) {
    if (eval(less(abs(x), policy.epsilon()))) return 0;
    return x < 0 ? -1 : 1;
}

template <class Op, class E, class T, class U,
          enable_int_if<is_comparison_operation_v<Op> &&
                        (std::is_floating_point_v<T> || std::is_floating_point_v<U>)> = 0>
inline constexpr auto perform_policy(Op op, const absolute_error<E>& policy, const T& lhs,
                                     const U& rhs) {
    return op(sgn(sub(lhs, rhs)), 0);
}

/**
 * Comparable square root with deferred call to sqrt.
 */
template <class T>
class square_root {
public:
    constexpr explicit square_root(const T& value = {}) : sqr_{value} {}

    operator decltype(eval(sqrt(std::declval<T>())))() const { return eval(sqrt(sqr_)); }

    friend constexpr const T& perform(Sqr, const square_root& x) { return x.sqr_; }

    template <class Op, enable_int_if<is_comparison_operation_v<Op>> = 0>
    friend constexpr auto perform(Op op, const square_root& lhs, const square_root& rhs) {
        return op(lhs.sqr_, rhs.sqr_);
    }

private:
    T sqr_;
};

struct defer_sqrt : virtual policy {};

template <class T>
inline auto perform(Sqrt, defer_sqrt, const T& x) {
    return square_root{x};
}

}  // namespace ac::math
