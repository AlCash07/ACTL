/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operators.hpp>
#include <actl/util/type_traits.hpp>
#include <tuple>

namespace ac {
namespace op {

template <class B = none>
struct base : operators::base<B> {
    using operators::base<B>::base;
};

template <class... Ts>
using enable_adl = enable_int_if<(... || is_template_base_of_v<base, Ts>)>;

// Default operation policy.
struct policy {};

}  // namespace op

inline constexpr op::policy default_policy;

template <class T>
inline constexpr bool is_policy_v = std::is_base_of_v<op::policy, remove_cvref_t<T>>;

namespace op {

/* Argument traits */

struct scalar_tag {};

template <class T, class = void>
struct category_impl {
    using type = scalar_tag;
};

template <class T>
struct category : category_impl<T> {};

template <class T>
using category_t = typename category<remove_cvref_t<T>>::type;

struct arithmetic_tag : scalar_tag {};

template <class T>
struct category_impl<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
    using type = arithmetic_tag;
};

template <class T, class U>
struct is_wider_category : std::is_base_of<T, U> {};

namespace detail {

template <class Tag, int Depth>
struct cdp {  // category-depth pair
    using type = Tag;
};

template <class T0, int D0, class T1, int D1>
inline constexpr auto operator || (cdp<T0, D0> lhs, cdp<T1, D1> rhs) {
    if constexpr (D0 == D1) {
        if constexpr (std::is_same_v<T0, T1> || is_wider_category<T0, T1>::value) {
            return lhs;
        } else {
            static_assert(is_wider_category<T1, T0>::value,
                          "arguments have incompatible categories");
            return rhs;
        }
    } else if constexpr (D0 < D1) {
        return rhs;
    } else {
        return lhs;
    }
}

template <class T, class = void>
struct depth : index_constant<0> {};

template <class T>
struct depth<T, std::void_t<typename category_t<T>::has_nested>>
    : index_constant<depth<value_t<T>>::value + 1> {};

template <class... Ts>
using major_category = typename remove_cvref_t<decltype(
    (... || std::declval<cdp<category_t<Ts>, depth<remove_cvref_t<Ts>>::value>>()))>::type;

}  // namespace detail

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

template <class T>
using enable_int_if_operation = enable_int_if<is_operation_v<T>>;

/* Operation arity trait: defined by nested `static constexpr int arity = N;`. */

template <class T, class = void>
struct arity : index_constant<0> {};

template <class T>
struct arity<T, std::void_t<decltype(T::arity)>> : index_constant<T::arity> {};

template <class T>
inline constexpr index arity_v = arity<T>::value;

/* Operation is_associative trait: defined by nested `struct is_associative;`. */

template <class T, class = void>
struct is_associative : std::false_type {};

template <class T>
struct is_associative<T, std::void_t<typename T::is_associative>> : std::true_type {
    static_assert(arity_v<T> == 2, "only binary operation can be associative");
};

template <class T>
inline constexpr bool is_associative_v = is_associative<T>::value;

/* Operation is_commutative trait: defined by nested `struct is_commutative;`. */

template <class T, class = void>
struct is_commutative : std::false_type {};

template <class T>
struct is_commutative<T, std::void_t<typename T::is_commutative>> : std::true_type {
    static_assert(arity_v<T> == 2, "only binary operation can be commutative");
};

template <class T>
inline constexpr bool is_commutative_v = is_commutative<T>::value;

/* Operation execution */

template <class... Ts>
using result_t = decltype(eval(std::declval<Ts>()...));

template <class T>
struct is_expression : std::false_type {};

template <class T>
inline constexpr bool is_expression_v = is_expression<remove_cvref_t<T>>::value;

template <class, class... Ts>
struct can_perform : std::false_type {};

// Drop policy if operation isn't specialized for it.
template <class Policy, class... Ts,
          enable_int_if<is_policy_v<Policy> && !can_perform<void, Policy, Ts...>::value> = 0>
inline constexpr decltype(auto) perform(const Policy&, const Ts&... xs) {
    return perform(xs...);
}

template <class, class... Ts>
struct has_fallback : std::false_type {};

template <class Op, class... Ts>
struct has_fallback<std::void_t<decltype(Op::perform(std::declval<Ts>()...))>, Op, Ts...>
    : std::bool_constant<!is_expression_v<decltype(Op::perform(std::declval<Ts>()...))>> {};

// Default evaluation provided by operation.
template <class Op, class... Ts,
          enable_int_if<is_operation_v<Op> && has_fallback<void, Op, Ts...>::value> = 0>
inline constexpr auto perform(Op, const Ts&... xs) {
    return Op::perform(xs...);
}

template <class... Ts>
struct can_perform<std::void_t<decltype(perform(std::declval<Ts>()...))>, Ts...> : std::true_type {
};

template <class Policy, class T, enable_int_if<!is_expression_v<T>> = 0>
inline T eval(const Policy& policy, T&& x) {
    return std::forward<T>(x);
}

template <class Policy, class Op, class... Ts>
inline constexpr decltype(auto) eval_dispatch(const Policy& policy, Op op, const Ts&... xs) {
    return eval(detail::major_category<result_t<Policy, const Ts&>...>{}, policy, op, xs...);
}

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

    using result_type = decltype(eval_dispatch(default_policy, Op{}, std::declval<Ts>()...));

    operator result_type() const { return eval(*this); }
};

template <class... Ts>
struct is_expression<expression<Ts...>> : std::true_type {};

template <class T, class U = remove_cvref_t<T>>
using value_if_arithmetic = std::conditional_t<std::is_arithmetic_v<U>, U, T>;

template <class Op, class... Ts>
inline auto make_expression(Ts&&... xs) {
    return expression<Op, std::make_index_sequence<sizeof...(Ts)>, value_if_arithmetic<Ts>...>{
        {std::forward<Ts>(xs)...}};
}

template <class Policy, class Op, size_t... Is, class... Ts>
inline decltype(auto) eval(const Policy& policy,
                           const expression<Op, std::index_sequence<Is...>, Ts...>& e) {
    return eval_dispatch(policy, Op{}, std::get<Is>(e.args)...);
}

template <class Op, class S, class... Ts>
inline decltype(auto) eval(const expression<Op, S, Ts...>& e) {
    return eval(default_policy, e);
}

// Base class for operations.
template <class Derived, int Arity>
struct operation {
    using operation_tag = base_operation_tag;

    static constexpr int arity = Arity;

    template <class... Ts>
    decltype(auto) operator()(Ts&&... xs) const {
        static_assert(sizeof...(Ts) > 0);
        if constexpr (is_policy_v<nth_t<0, Ts...>>) {
            return dispatch(xs...);
        }
        if constexpr (arity == sizeof...(Ts) && inplace_argument_count_v<Ts...> == 0) {
            return make_expression<Derived>(std::forward<Ts>(xs)...);
        } else {
            // static_assert(false);
        }
    }

private:
    template <class Policy, class... Ts>
    decltype(auto) dispatch(const Policy& policy, const Ts&... xs) const {
        if constexpr (sizeof...(Ts) == 0) {
            // Operation with only policy argument results in a function object.
            return [&policy](auto&&... xs) { return eval_dispatch(policy, Derived{}, xs...); };
        } else {
            return eval_dispatch(policy, Derived{}, xs...);
        }
    }
};

}  // namespace op

template <class T>
inline op::inplace_argument<T> inplace(T&& x) {
    return {std::forward<T>(x)};
}

}  // namespace ac
