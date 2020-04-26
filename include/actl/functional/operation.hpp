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

using operators::pass;

// Default operation policy.
struct policy {};

}  // namespace op

inline constexpr op::policy default_policy;

template <class T>
inline constexpr bool is_policy_v = std::is_base_of_v<op::policy, remove_cvref_t<T>>;

/* Output and inplace parameters */

template <bool In, class T>
struct out {
    T x;

    explicit out(T&& x) : x{std::forward<T>(x)} {}
};

template <class T>
out(T&&) -> out<false, T>;

template <class T>
inline auto inplace(T&& x) {
    return out<true, T>{std::forward<T>(x)};
}

namespace op {

/* Argument traits */

template <class T>
struct is_out : std::false_type {};

template <bool In, class T>
struct is_out<out<In, T>> : std::true_type {};

template <class T>
struct raw {
    using type = T;
};

template <class T>
using raw_t = typename raw<remove_cvref_t<T>>::type;

template <bool In, class T>
struct raw<out<In, T>> {
    using type = raw_t<T>;
};

struct scalar_tag {};

template <class T, class = void>
struct category_impl {
    using type = scalar_tag;
};

template <class T>
struct category : category_impl<T> {};

template <class T>
using category_t = typename category<raw_t<T>>::type;

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
    (... || std::declval<cdp<category_t<Ts>, depth<raw_t<Ts>>::value>>()))>::type;

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

/* Operation is_associative trait: defined by nested `struct is_associative;`. */

template <class T, class = void>
struct is_associative : std::false_type {};

template <class T>
struct is_associative<T, std::void_t<typename T::is_associative>> : std::true_type {};

template <class T>
inline constexpr bool is_associative_v = is_associative<T>::value;

/* Operation is_commutative trait: defined by nested `struct is_commutative;`. */

template <class T, class = void>
struct is_commutative : std::false_type {};

template <class T>
struct is_commutative<T, std::void_t<typename T::is_commutative>> : std::true_type {};

template <class T>
inline constexpr bool is_commutative_v = is_commutative<T>::value;

/* Operation execution */

template <class... Ts>
using result_t = decltype(eval(std::declval<Ts>()...));

template <class T>
struct is_expression : std::false_type {};

template <class T>
inline constexpr bool is_expression_v = is_expression<remove_cvref_t<T>>::value;

template <class Op, class... Ts>
inline constexpr auto perform(Op op, const Ts&... xs) -> decltype(Op::perform(xs...)) {
    return Op::perform(xs...);
}

// Drop policy if operation isn't specialized for it.
template <class Op, class... Ts>
inline constexpr auto perform_policy(Op op, policy, const Ts&... xs) -> decltype(perform(op, xs...)) {
    return perform(op, xs...);
}

template <class Policy, class T, enable_int_if<!is_expression_v<T>> = 0>
inline T eval(const Policy& policy, T&& x) {
    return pass<T>(x);
}

template <class Policy, class T>
inline decltype(auto) eval(const Policy& policy, const out<true, T>& x) {
    return eval(policy, x.x);
}

template <class Tag, class Op, class = void>
struct calculator;

namespace detail {

template <class... Ts>
struct calc;

template <class Op, class Policy, class... Ts>
struct calc<Op, Policy, Ts...> {
    using type = calculator<detail::major_category<result_t<Policy, const Ts&>...>, Op>;
};

}  // namespace detail

template <class... Ts>
using calculator_t = typename detail::calc<Ts...>::type;

/* Expression */

namespace detail {

template <bool, class... Ts>
struct expr_result {
    using type = void;
};

template <class Op, class... Ts>
struct expr_result<true, Op, Ts...> {
    using type = decltype(calculator_t<Op, Ts...>::evaluate(std::declval<Ts>()...));
};

template <bool, class Op, class... Ts>
struct expr_traits {
    using C = calculator_t<Op, Ts...>;

    static constexpr bool can_eval = decltype(C::can_eval(std::declval<Ts>()...))::value;

    using type = typename expr_result<can_eval, Op, Ts...>::type;
};

template <class Op, class... Ts>
struct expr_traits<false, Op, Ts...> : expr_traits<true, Op, policy, Ts...> {};

}  // namespace detail

// S is always std::make_index_sequence<sizeof...(Ts)>> to simplify arguments traversal.
template <class Op, class S, class... Ts>
struct expression : operators::base<> {
    std::tuple<Ts...> args;

    static constexpr bool has_policy = is_policy_v<nth_t<0, Ts...>>;

    using traits = detail::expr_traits<has_policy, Op, Ts...>;

    template <bool B = traits::can_eval, enable_int_if<B> = 0>
    operator typename traits::type() const {
        return eval(*this);
    }
};

template <class... Ts>
struct is_expression<expression<Ts...>> : std::true_type {};

template <class T, class U = remove_cvref_t<T>>
using value_if_arithmetic = std::conditional_t<std::is_arithmetic_v<U>, U, T>;

template <class Op, class... Ts>
inline auto make_expression(Ts&&... xs) {
    return expression<Op, std::make_index_sequence<sizeof...(Ts)>, value_if_arithmetic<Ts>...>{
        {}, {std::forward<Ts>(xs)...}};
}

template <class Policy, class Op, size_t... Is, class... Ts>
inline decltype(auto) eval(const Policy& policy,
                           const expression<Op, std::index_sequence<Is...>, Ts...>& e) {
    if constexpr (expression<Op, std::index_sequence<Is...>, Ts...>::has_policy) {
        return calculator_t<Op, Ts...>::evaluate(std::get<Is>(e.args)...);
    } else {
        return calculator_t<Op, Policy, Ts...>::evaluate(policy, std::get<Is>(e.args)...);
    }
}

template <class... Ts>
inline decltype(auto) eval(const expression<Ts...>& e) {
    return eval(default_policy, e);
}

// Base class for operations.
template <class Derived>
struct operation {
    using operation_tag = base_operation_tag;

    template <class... Ts>
    decltype(auto) operator()(Ts&&... xs) const {
        static_assert(sizeof...(Ts) > 0);
        constexpr bool have_policy = is_policy_v<nth_t<0, Ts...>>;
        if constexpr ((... || is_out<remove_cvref_t<Ts>>::value)) {
            if constexpr (have_policy) {
                return calculator_t<Derived, Ts...>::assign(xs...);
            } else {
                return calculator_t<Derived, policy, Ts...>::assign(default_policy, xs...);
            }
        } else {
            if constexpr (have_policy && sizeof...(Ts) == 1) {
                return make_function(xs...);
            } else {
                return make_expression<Derived>(pass<Ts>(xs)...);
            }
        }
    }

private:
    template <class Policy>
    auto make_function(const Policy& policy) const {
        return [&policy](auto&&... xs) { 
            return calculator_t<Derived, Policy, decltype(xs)...>::evaluate(policy, xs...); 
        };
    }
};

}  // namespace op
}  // namespace ac
