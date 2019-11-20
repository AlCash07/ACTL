/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/type_traits.hpp>

namespace ac::op {

// Default operation policy.
struct default_policy {
    struct is_policy;
};

/* is_policy trait: defined by nested `struct is_policy;`. */

template <class T, class = void>
struct is_policy : std::false_type {};

template <class T>
struct is_policy<T, std::void_t<typename T::is_policy>> : std::true_type {};

template <class T>
inline constexpr bool is_policy_v = is_policy<std::remove_reference_t<T>>::value;

// Perform without policy that applies default policy.
template <class Operation, class T, class... Ts, std::enable_if_t<!is_policy_v<T>, int> = 0>
inline decltype(auto) perform(Operation, T&& x, Ts&&... xs) {
    return perform(Operation{}, default_policy{}, std::forward<T>(x), std::forward<Ts>(xs)...);
}

// Base class for operations.
template <class Derived>
struct operation {
    struct is_operation;

    template <class... Ts>
    inline decltype(auto) operator()(Ts&&... xs) const {
        return perform(Derived{}, std::forward<Ts>(xs)...);
    }
};

/* is_operation trait: satisfied by deriving from operation. */

template <class T, class = void>
struct is_operation : std::false_type {};

template <class T>
struct is_operation<T, std::void_t<typename T::is_operation>> : std::true_type {};

template <class T>
inline constexpr bool is_operation_v = is_operation<T>::value;

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

}  // namespace ac::op
