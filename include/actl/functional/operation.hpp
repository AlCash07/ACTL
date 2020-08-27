/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/type_traits.hpp>
#include <tuple>

namespace ac::math {

/* Argument traits */

template <class T>
struct raw {
    using type = T;
};

template <class T>
using raw_t = typename raw<remove_cvref_t<T>>::type;

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
    static constexpr int value = Depth;
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
using major_cdp =
    remove_cvref_t<decltype((... || std::declval<cdp<category_t<Ts>, type_depth_v<Ts>>>()))>;

template <class... Ts>
using major_category = typename major_cdp<Ts...>::type;

template <bool B, class T>
struct value_if {
    using type = value_t<raw_t<T>>;
};

template <class T>
struct value_if<false, T> {
    using type = T;
};

template <bool B, class T>
using value_if_t = typename value_if<B, T>::type;

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

template <class T>
inline constexpr T eval(const T& x) {
    return x;
}

template <class T, size_t N>
inline auto& eval(const T (&x)[N]) {
    return x;
}

/* Expression */

// Base class for operations.
template <class Derived>
struct operation;

namespace detail {

template <class Op, class... Ts>
struct result {
    using type = decltype(std::declval<Op>().evaluate(std::declval<Ts>()...));
    using tag = category_t<type>;
};

}  // namespace detail

template <class Derived, class Tag>
struct expression_base {};

template <class... Ts>
struct expression : expression_base<expression<Ts...>, typename detail::result<Ts...>::tag> {
    std::tuple<Ts...> args;

    constexpr operator typename detail::result<Ts...>::type() const { return eval(*this); }
};

template <class... Ts>
struct expression_op : operation<expression_op<Ts...>> {
    std::tuple<Ts...> args;

    template <class... Us>
    explicit constexpr expression_op(Us&&... xs) : args{std::forward<Us>(xs)...} {}

    template <class... Us>
    constexpr auto evaluate(const Us&... xs) const {
        return eval(expand_expression(*this, xs...));
    }

    template <class T, class... Us>
    constexpr void assign(T& dst, const Us&... xs) const {
        out(dst) = expand_expression(*this, xs...);
    }
};

template <class T, class U = remove_cvref_t<T>>
using value_if_small = std::conditional_t<std::is_empty_v<U> || std::is_arithmetic_v<U>, U, T>;

template <class... Ts>
inline constexpr auto make_expression(Ts&&... xs) {
    if constexpr ((... + is_operation_v<Ts>) == 1) {
        return expression<value_if_small<Ts>...>{{}, {std::forward<Ts>(xs)...}};
    } else {
        return expression_op<value_if_small<Ts>...>{std::forward<Ts>(xs)...};
    }
}

template <class Op, class... Ts>
inline constexpr decltype(auto) expand_expression(const Op& op, const Ts&... xs) {
    if constexpr (!is_operation_v<Op>) {
        return op;
    } else {
        return op(xs...);
    }
}

template <class... Ts>
struct expression_helper : expression_helper<std::make_index_sequence<sizeof...(Ts) - 1>> {};

template <size_t... Is>
struct expression_helper<std::index_sequence<Is...>> {
    template <class EO, class... Us>
    static constexpr auto expand_impl(const EO& eo, const Us&... xs) {
        return make_expression(std::get<0>(eo.args),
                               expand_expression(std::get<Is + 1>(eo.args), xs...)...);
    }

    template <class E>
    static constexpr decltype(auto) eval_impl(const E& e) {
        return eval(std::get<0>(e.args).evaluate(std::get<Is + 1>(e.args)...));
    }

    template <class T, class E>
    static constexpr void assign_impl(T& dst, const E& e) {
        std::get<0>(e.args)(dst, std::get<Is + 1>(e.args)...);
    }
};

template <class... Ts, class... Us>
inline constexpr auto expand_expression(const expression_op<Ts...>& eo, const Us&... xs) {
    return expression_helper<Ts...>::expand_impl(eo, xs...);
}

template <class... Ts>
inline constexpr decltype(auto) eval(const expression<Ts...>& e) {
    return expression_helper<Ts...>::eval_impl(e);
}

template <class... Ts>
struct category_impl<expression<Ts...>> {
    using type = typename detail::result<Ts...>::tag;
};

/* Output and inplace arguments */

template <bool In, class T>
struct out {
    T x;

    constexpr operator T() { return x; }

    template <class U>
    out& operator=(const U& y);

    template <class... Ts>
    out& operator=(const expression<Ts...>& e) {
        static_assert(!In, "use output instead of inplace argument for assignment");
        expression_helper<Ts...>::assign_impl(*this, e);
        return *this;
    }
};

template <class T, bool NotIn = false>
struct is_out : std::false_type {};

template <bool In, class T, bool NotIn>
struct is_out<out<In, T>, NotIn> : std::bool_constant<!(NotIn && In)> {};

template <bool In, class T>
struct raw<out<In, T>> {
    using type = raw_t<T>;
};

template <class T>
inline constexpr const T& remove_inplace(const out<true, T>& x) {
    return x.x;
}

template <class T>
inline constexpr const T& remove_inplace(const T& x) {
    return x;
}

template <class T, class... Ts>
inline constexpr auto& find_dst(T&, Ts&... xs) {
    return find_dst(xs...);
}

template <class T, class... Ts>
inline constexpr T& find_dst(out<true, T>& x, Ts&... xs) {
    return x.x;
}

/* Policy support */

template <class T, class = void>
struct is_policy : std::false_type {};

template <class T>
struct is_policy<T, std::void_t<typename T::is_policy>> : std::true_type {};

template <class T>
inline constexpr bool is_policy_v = is_policy<T>::value;

template <class Op, class Policy, class = void>
struct can_apply_policy : std::false_type {};

template <class Op, class Policy>
struct can_apply_policy<
    Op, Policy, std::void_t<decltype(apply_policy(std::declval<Op>(), std::declval<Policy>()))>>
    : std::true_type {};

/* Operation overload resolution */

template <class Op, class = void>
struct default_operation_resolver {
    static constexpr const Op& resolve(const Op& op) { return op; }
};

template <class Op>
struct default_operation_resolver<Op, std::void_t<decltype(Op::formula)>> {
    static constexpr auto resolve(Op) { return Op::formula; }
};

template <class Op, class Category, class... Ts>
struct operation_resolver : default_operation_resolver<Op> {};

template <class Op, class Policy>
struct tuned_operation : operation<tuned_operation<Op, Policy>> {
    std::tuple<Op, Policy> t;

    template <class... Ts>
    explicit tuned_operation(Ts&&... xs) : t{std::forward<Ts>(xs)...} {}
};

template <class... Ts>
struct operation_resolver_helper {
    template <class Op>
    static constexpr decltype(auto) resolve(const Op& op) {
        return operation_resolver<Op, detail::major_category<Ts...>, raw_t<Ts>...>::resolve(op);
    }

    template <class Op, class Policy>
    static constexpr decltype(auto) resolve(const tuned_operation<Op, Policy>& op) {
        decltype(auto) resolved_op = resolve(std::get<0>(op.t));
        if constexpr (can_apply_policy<decltype(resolved_op), Policy>::value) {
            return apply_policy(resolved_op, std::get<1>(op.t));
        } else {
            return std::forward<decltype(resolved_op)>(resolved_op);
        }
    }
};

template <class Derived>
struct operation {
    using category = operation_tag;

    constexpr const Derived& derived() const { return static_cast<const Derived&>(*this); }

    template <class... Ts, class T = remove_cvref_t<nth_t<0, Ts...>>,
              enable_int_if<!is_out<T, true>::value && !is_policy_v<T>> = 0>
    constexpr decltype(auto) operator()(Ts&&... xs) const {
        decltype(auto) op = resolve<Ts...>();
        if constexpr ((... || is_out<remove_cvref_t<Ts>>::value)) {
            static_assert(1 == (... + is_out<remove_cvref_t<Ts>>::value),
                          "single inplace argument expected");
            auto& dst = find_dst(xs...);
            op.assign(dst, remove_inplace(xs)...);
            return dst;
        } else {
            return make_expression(std::forward<decltype(op)>(op), std::forward<Ts>(xs)...);
        }
    }

    template <class T, class... Ts>
    constexpr T operator()(out<false, T> dst, const Ts&... xs) const {
        resolve<T, Ts...>().assign(dst.x, xs...);
        return dst.x;
    }

    template <class Policy, enable_int_if<is_policy_v<Policy>> = 0>
    constexpr auto operator()(Policy&& policy) const {
        return tuned_operation<Derived, Policy>{derived(), std::forward<Policy>(policy)};
    }

    template <class... Ts>
    constexpr decltype(auto) resolve() const {
        return operation_resolver_helper<Ts...>::resolve(derived());
    }

    template <class... Ts>
    constexpr decltype(auto) resolve_nested() const {
        return operation_resolver_helper<
            detail::value_if_t<detail::type_depth_v<Ts> == detail::major_cdp<Ts...>::value,
                               Ts>...>::resolve(derived());
    }
};

template <class OuterOp, class InnerOp>
struct composite_operation : operation<composite_operation<OuterOp, InnerOp>>, private InnerOp {
    using category = operation_tag;

    template <class... Ts>
    explicit composite_operation(Ts&&... xs) : InnerOp{std::forward<Ts>(xs)...} {}

    constexpr const InnerOp& inner() const { return static_cast<const InnerOp&>(*this); }

    using operation<composite_operation<OuterOp, InnerOp>>::operator();

    template <class... Ts>
    constexpr auto evaluate(const Ts&... xs) const {
        return OuterOp::evaluate(inner(), xs...);
    }
};

template <class OuterOp>
struct operation_composer {
    template <class InnerOp>
    constexpr auto operator()(InnerOp&& op) const {
        return composite_operation<OuterOp, remove_cvref_t<InnerOp>>{std::forward<InnerOp>(op)};
    }
};

template <class Outer, class Inner, class Policy>
inline constexpr auto apply_policy(const composite_operation<Outer, Inner>& op,
                                   const Policy& policy)
    -> decltype(operation_composer<Outer>{}(apply_policy(op.inner(), policy))) {
    return operation_composer<Outer>{}(apply_policy(op.inner(), policy));
}

}  // namespace ac::math

namespace ac {

template <class T>
inline math::out<false, T> out(T&& x) {
    return {std::forward<T>(x)};
}

template <class T>
inline math::out<true, T> inplace(T&& x) {
    return {std::forward<T>(x)};
}

}  // namespace ac
