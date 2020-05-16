/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/type_traits.hpp>
#include <tuple>

namespace ac {
namespace math {

// Default operation policy.
struct policy {};

}  // namespace math

inline constexpr math::policy default_policy;

template <class T>
inline constexpr bool is_policy_v = std::is_base_of_v<math::policy, remove_cvref_t<T>>;

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

namespace math {

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
struct category_impl<T, std::void_t<typename T::category>> {
    using type = typename T::category;
};

template <class T>
struct category : category_impl<T> {};

template <class T>
using category_t = typename category<raw_t<T>>::type;

namespace detail {

template <class Tag, class = void>
struct category_depth : index_constant<0> {};

template <class T>
inline constexpr index category_depth_v = category_depth<T>::value;

template <class Tag>
struct category_depth<Tag, std::void_t<typename Tag::base>> 
    : index_constant<1 + category_depth_v<typename Tag::base>> {};

template <class T, class U>
inline constexpr auto common_category(T, U) {
    constexpr index DT = category_depth_v<T>;
    constexpr index DU = category_depth_v<U>;
    if constexpr (DT == DU) {
        if constexpr (std::is_same_v<T, U>) {
            return T{};
        } else {
            static_assert(DT > 0, "incompatible categories");
            return common_category(typename T::base{}, typename U::base{});
        }
    } else if constexpr (DT < DU) {
        return common_category(T{}, typename U::base{});
    } else {
        return common_category(typename T::base{}, U{});
    }
}

template <class Tag, int Depth>
struct cdp {  // category-depth pair
    using type = Tag;
};

template <class T, int DT, class U, int DU>
inline constexpr auto operator || (cdp<T, DT> lhs, cdp<U, DU> rhs) {
    if constexpr (DT == DU) {
        return cdp<decltype(common_category(T{}, U{})), DT>{};
    } else if constexpr (DT < DU) {
        return rhs;
    } else {
        return lhs;
    }
}

template <class T, class = void>
struct type_depth : index_constant<0> {};

template <class T>
inline constexpr index type_depth_v = type_depth<raw_t<T>>::value;

template <class T>
struct type_depth<T, std::void_t<typename category_t<T>::has_nested>>
    : index_constant<type_depth_v<value_t<T>> + 1> {};

template <class... Ts>
using major_category = typename remove_cvref_t<decltype(
    (... || std::declval<cdp<category_t<Ts>, type_depth_v<Ts>>>()))>::type;

}  // namespace detail

struct operation_tag {};

template <class T>
inline constexpr bool is_operation_v = std::is_same_v<operation_tag, category_t<T>>;

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

// pass is the same as std::forward except it converts reference into const reference
template <class T>
inline constexpr T&& pass(std::remove_reference_t<T>& x) {
    return static_cast<T&&>(x);
}

template <class T>
inline constexpr const T&& pass(std::remove_reference_t<T>&& x) {
    static_assert(!std::is_lvalue_reference_v<T>, "can not pass an rvalue as an lvalue");
    return static_cast<const T&&>(x);
}

template <class Op, class... Ts>
inline constexpr auto perform(Op op, const Ts&... xs) -> decltype(Op::perform(xs...)) {
    return Op::perform(xs...);
}

// Drop policy if operation isn't specialized for it.
template <class Op, class... Ts>
inline constexpr auto perform_policy(Op op, policy, const Ts&... xs)
    -> decltype(perform(op, xs...)) {
    return perform(op, xs...);
}

template <class T, class... Ts>
inline constexpr T eval(const T& x, const Ts&...) {
    return x;
}

template <class T, size_t N, class... Ts>
inline auto& eval(const T (&x)[N], const Ts&...) {
    return x;
}

template <class T, class... Ts>
inline decltype(auto) eval(const out<true, T>& x, const Ts&...) {
    return x.x;
}

namespace detail {

template <class... Ts>
struct calc;

template <class Op, class Policy, class... Ts>
struct calc<Op, Policy, Ts...> {
    using type = decltype(get_calculator(Op{}, major_category<result_t<const Ts&, Policy>...>{}));
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

template <class Op, class... Ts>
struct expr_traits {
    using C = calculator_t<Op, Ts...>;

    static constexpr bool can_eval = decltype(C::can_eval(std::declval<Ts>()...))::value;

    using type = typename expr_result<can_eval, Op, Ts...>::type;
};

}  // namespace detail

// S is always std::make_index_sequence<sizeof...(Ts)>> to simplify arguments traversal.
template <class Op, class S, class... Ts>
struct expression {
    std::tuple<Ts...> args;

    using traits = detail::expr_traits<Op, policy, Ts...>;

    template <class... Us>
    explicit constexpr expression(Us&&... xs) : args{std::forward<Us>(xs)...} {}

    template <bool B = traits::can_eval, enable_int_if<B> = 0>
    operator typename traits::type() const {
        return eval(*this);
    }
};

template <class T, class U = remove_cvref_t<T>>
using value_if_arithmetic = std::conditional_t<std::is_arithmetic_v<U>, U, T>;

template <class Op, class... Ts>
inline constexpr auto make_expression(Ts&&... xs) {
    return expression<Op, std::make_index_sequence<sizeof...(Ts)>, value_if_arithmetic<Ts>...>{
        std::forward<Ts>(xs)...};
}

template <class Op, size_t... Is, class... Ts, class Policy>
inline constexpr decltype(auto) eval(const expression<Op, std::index_sequence<Is...>, Ts...>& e,
                                     const Policy& policy) {
    return calculator_t<Op, Policy, Ts...>::evaluate(policy, std::get<Is>(e.args)...);
}

template <class... Ts>
inline constexpr decltype(auto) eval(const expression<Ts...>& e) {
    return eval(e, default_policy);
}

// Base class for operations.
template <class Derived, index Arity, class ArgumentsTag>
struct operation {
    using category = operation_tag;
    using arguments_tag = ArgumentsTag;

    static constexpr index arity = Arity;

    template <class... Ts>
    static constexpr bool can_perform = ((sizeof...(Ts) == arity && are_same_v<Ts...>)&&... &&
                                         std::is_base_of_v<arguments_tag, category_t<Ts>>);

    template <class... Ts>
    constexpr decltype(auto) operator()(Ts&&... xs) const {
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

}  // namespace math
}  // namespace ac
