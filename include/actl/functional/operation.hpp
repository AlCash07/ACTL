/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/none.hpp>
#include <actl/util/type_traits.hpp>
#include <tuple>

namespace ac {
namespace op {

// Inherit from this class to enable ADL lookup to find operators in this namespace.
// Template base class enables empty base class chaining to avoid type size increasing,
// reference: https://www.boost.org/doc/libs/1_70_0/libs/utility/operators.htm#old_lib_note
template <class B = none>
struct base : B {
    using B::B;
};

// Default operation policy.
struct policy {};

}  // namespace op

inline constexpr op::policy default_policy;

/* is_policy trait: defined by nested `struct is_policy;`. */

template <class T>
inline constexpr bool is_policy_v = std::is_base_of_v<op::policy, remove_cvref_t<T>>;

template <class T>
using enable_int_if_policy = enable_int_if<is_policy_v<T>>;

template <class T>
using disable_int_if_policy = enable_int_if<!is_policy_v<T>>;

namespace op {

template <class Policy, class T>
inline auto& eval(const Policy& policy, const T& x) {
    return x;
}

template <class... Ts>
struct result_type {
    using type = decltype(eval(std::declval<Ts>()...));
};

template <class... Ts>
using result_t = typename result_type<Ts...>::type;

template <class... Ts>
struct have_policy : std::bool_constant<is_policy_v<nth_t<0, Ts...>>> {};

template <>
struct have_policy<> : std::false_type {};

template <class... Ts>
inline constexpr bool have_policy_v = have_policy<Ts...>::value;

template <class... Ts>
inline constexpr index argument_count_v = sizeof...(Ts) - have_policy_v<Ts...>;

struct base_operation_tag {};

/* Every operation mush define nested `struct operation_tag;`. */

template <class T, class = void>
struct operation_tag {
    using type = void;
};

template <class T>
struct operation_tag<T, std::void_t<typename T::operation_tag>> {
    using type = typename T::operation_tag;
};

template <class T>
using operation_tag_t = typename operation_tag<T>::type;

template <class T>
inline constexpr bool is_operation_v = !std::is_same<operation_tag_t<T>, void>::value;

/* Operation arity trait: defined by nested `static constexpr int arity = N;`. */

template <class T, class = void>
struct arity : index_constant<0> {};

template <class T>
struct arity<T, std::void_t<decltype(T::arity)>> : index_constant<T::arity> {};

template <class T>
inline constexpr index arity_v = arity<T>::value;

/* Operation is_commutative trait: defined by nested `struct is_commutative;`. */

template <class T, class = void>
struct is_commutative : std::false_type {};

template <class T>
struct is_commutative<T, std::void_t<typename T::is_commutative>> : std::true_type {
    static_assert(arity_v<T> == 2, "only binary operation can be commutative");
};

template <class T>
inline constexpr bool is_commutative_v = is_commutative<T>::value;

/* Operation is_associative trait: defined by nested `struct is_associative;`. */

template <class T, class = void>
struct is_associative : std::false_type {};

template <class T>
struct is_associative<T, std::void_t<typename T::is_associative>> : std::true_type {
    static_assert(arity_v<T> == 2, "only binary operation can be associative");
};

template <class T>
inline constexpr bool is_associative_v = is_associative<T>::value;

/* Inplace argument support */

template <class T>
struct inplace_argument {
    T x;
};

template <class T>
struct is_inplace : std::false_type {};

template <class T>
struct is_inplace<inplace_argument<T>> : std::true_type {};

template <class... Ts>
inline constexpr index inplace_argument_count_v = (... + is_inplace<Ts>::value);

template <class Policy, class T>
inline decltype(auto) eval(const Policy& policy, const inplace_argument<T>& x) {
    return eval(policy, x.x);
}

/* Expression */

// S is always std::make_index_sequence<sizeof...(Ts)>> to simplify arguments traversal.
template <class Op, class S, class... Ts>
struct expression {
    std::tuple<Ts...> args;

    using result_type = result_t<policy, Op, result_t<policy, Ts>...>;

    operator result_type() const { return eval(*this); }
};

template <class Policy, class Op, class S, class... Ts>
struct result_type<Policy, const expression<Op, S, Ts...>&> {
    using type = typename expression<Op, S, Ts...>::result_type;
};

template <class T, class U = remove_cvref_t<T>>
using value_if_arithmetic = std::conditional_t<std::is_arithmetic_v<U>, U, T>;

template <class Op, class... Ts>
inline auto make_expression(Ts&&... xs) {
    return expression<Op, std::make_index_sequence<sizeof...(Ts)>, value_if_arithmetic<Ts>...>{
        {std::forward<Ts>(xs)...}};
}

template <class Op, class... Ts>
inline constexpr bool make_expr_v = inplace_argument_count_v<Ts...> == 0 &&
                                    arity_v<Op> == sizeof...(Ts);

template <class Op, class... Ts, enable_int_if<make_expr_v<Op, Ts...>> = 0>
inline auto dispatch(Op, Ts&&... xs) {
    return make_expression<Op>(std::forward<Ts>(xs)...);
}

template <class Policy, class Op, size_t... Is, class... Ts>
inline decltype(auto) eval(const Policy& policy,
                           const expression<Op, std::index_sequence<Is...>, Ts...>& e) {
    return eval(policy, Op{}, eval(policy, std::get<Is>(e.args))...);
}

template <class Op, class S, class... Ts>
inline decltype(auto) eval(const expression<Op, S, Ts...>& e) {
    return eval(default_policy, e);
}

/* Operation perform */

template <class, class... Ts>
struct can_perform : std::false_type {};

template <class... Ts>
inline constexpr bool can_perform_v = can_perform<void, Ts...>::value;

template <class... Ts>
using enable_int_if_can_perform = enable_int_if<can_perform_v<Ts...>>;

// Applies default policy if no policy is specified.
// template <class Op, class... Ts, enable_int_if<!make_expr_v<Op, Ts...> && !have_policy_v<Ts...>>
// = 0> inline constexpr decltype(auto) perform(Op op, Ts&&... xs) {
//     return perform(op, default_policy, std::forward<Ts>(xs)...);
// }

// Operation with only policy argument results in a function object.
template <class Policy, class Op, enable_int_if<is_operation_v<Op>> = 0>
inline constexpr auto dispatch(const Policy& policy, Op) {
    return
        [&policy](auto&&... xs) { return eval(policy, Op{}, std::forward<decltype(xs)>(xs)...); };
}

// Default evaluation provided by operation.
template <class Op, class Policy, class... Ts>
inline constexpr auto perform(Op, const Policy&, const Ts&... xs) -> decltype(Op::eval(xs...)) {
    return Op::eval(xs...);
}

template <class Op, class T>
struct cast_before : virtual policy {};

template <class Op, class T, class... Ts>
inline constexpr auto perform(Op op, cast_before<Op, T>, const Ts&... xs)
    -> decltype(perform(op, default_policy, static_cast<T>(xs)...)) {
    return perform(op, default_policy, static_cast<T>(xs)...);
}

template <class... Ts>
struct can_perform<std::void_t<decltype(perform(std::declval<Ts>()...))>, Ts...> : std::true_type {};

// Base class for operations.
template <class Derived, int Arity>
struct operation {
    using operation_tag = base_operation_tag;

    static constexpr int arity = Arity;

    template <class T, class... Ts>
    inline decltype(auto) operator()(T&& x, Ts&&... xs) const {
        if constexpr (is_policy_v<T>) {
            return dispatch(x, Derived{}, std::forward<Ts>(xs)...);
        } else {
            return dispatch(Derived{}, std::forward<Ts>(xs)...);
        }
    }
};

struct scalar_operation_tag : base_operation_tag {};

template <class Derived, int Arity>
struct scalar_operation : operation<Derived, Arity> {
    using operation_tag = scalar_operation_tag;
};

}  // namespace op

template <class T>
inline op::inplace_argument<T> inplace(T&& x) {
    return {std::forward<T>(x)};
}

}  // namespace ac
