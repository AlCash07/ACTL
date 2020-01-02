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

/* Operation perform */

template <class, class... Ts>
struct can_perform : std::false_type {};

template <class... Ts>
inline constexpr bool can_perform_v = can_perform<void, Ts...>::value;

template <class... Ts>
using enable_int_if_can_perform = enable_int_if<can_perform_v<Ts...>>;

// Uses fallback implementation if there's no other way.
template <class Op, class Policy, class... Ts, enable_int_if<!can_perform_v<Op, Policy, Ts...>> = 0>
inline constexpr auto perform(Op, const Policy&, Ts&&... xs)
    -> decltype(Op::fallback(std::forward<Ts>(xs)...)) {
    return Op::fallback(std::forward<Ts>(xs)...);
}

// Applies default policy if no policy is specified.
template <class Op, class T, class... Ts, disable_int_if_policy<T> = 0>
inline constexpr auto perform(Op op, T&& x, Ts&&... xs)
    -> decltype(perform(op, default_policy, std::forward<T>(x), std::forward<Ts>(xs)...)) {
    return perform(op, default_policy, std::forward<T>(x), std::forward<Ts>(xs)...);
}

// Operation with only policy argument results in a function object.
template <class Op, class Policy, enable_int_if_policy<Policy> = 0>
inline constexpr auto perform(Op, const Policy& policy) {
    return [&policy](auto&&... xs) {
        return perform(Op{}, policy, std::forward<decltype(xs)>(xs)...);
    };
}

template <class Op, class T>
struct cast_before : virtual policy {};

template <class Op, class T, class... Ts>
inline constexpr auto perform(Op op, cast_before<Op, T>, const Ts&... xs)
    -> decltype(perform(op, default_policy, static_cast<T>(xs)...)) {
    return perform(op, default_policy, static_cast<T>(xs)...);
}

template <class... Ts>
struct result_type {
    using type = decltype(perform(std::declval<Ts>()...));
};

template <class... Ts>
using result_t = typename result_type<Ts...>::type;

template <class... Ts>
struct can_perform<std::void_t<decltype(perform(std::declval<Ts>()...))>, Ts...> : std::true_type {};

// Base class for operations.
template <class Derived, int Arity>
struct operation {
    using operation_tag = base_operation_tag;

    static constexpr int arity = Arity;

    template <class... Ts>
    inline decltype(auto) operator()(Ts&&... xs) const {
        return perform(Derived{}, std::forward<Ts>(xs)...);
    }
};

struct scalar_operation_tag : base_operation_tag {};

// Base class for scalar operations.
template <class Derived, int Arity>
struct scalar_operation : operation<Derived, Arity> {
    using operation_tag = scalar_operation_tag;
};

/* Inplace argument support */

template <class T>
struct inplace_argument {
    T x;
};

template <class... Ts>
struct inplace_argument_index;

template <>
struct inplace_argument_index<> : index_constant<-1> {};

template <class T, class... Ts>
struct inplace_argument_index<inplace_argument<T>, Ts...> : index_constant<0> {
    static_assert(inplace_argument_index<Ts...>::value == -1,
                  "more than one inplace argument specified");
};

template <class T, class... Ts>
struct inplace_argument_index<T, Ts...> {
    static constexpr int next = inplace_argument_index<Ts...>::value;

    static constexpr int value = next == -1 ? -1 : next + 1;
};

template <size_t I, class T, class... Ts>
inline auto& get_ith(T& x, Ts&... xs) {
    if constexpr (I == 0) {
        return x;
    } else {
        return get_ith<I - 1>(xs...);
    }
}

template <class T>
inline decltype(auto) simplify(T&& x) {
    return std::forward<T>(x);
}

template <class T>
inline T simplify(inplace_argument<T> x) {
    return x.x;
}

}  // namespace op
}  // namespace ac
