// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/operation_base.hpp>
#include <cmath>

namespace ac {

#define AC_MATH_OPERATION1(Type, name, category)                             \
    struct Type;                                                             \
                                                                             \
    struct Type##Scalar : operation_base<Type##Scalar> {                     \
        using parent = Type;                                                 \
                                                                             \
        template<typename T>                                                 \
        static constexpr auto evaluate(T t) {                                \
            using std::name;                                                 \
            return name(t);                                                  \
        }                                                                    \
    };                                                                       \
    inline constexpr Type##Scalar name##_scalar;                             \
                                                                             \
    struct Type : operation_base<Type> {                                     \
        using operation_category = category;                                 \
                                                                             \
        template<typename T>                                                 \
            requires std::is_arithmetic_v<T>                                 \
        friend constexpr auto specialization(Type, type_array<T>) noexcept { \
            return name##_scalar;                                            \
        }                                                                    \
    };                                                                       \
    inline constexpr Type name;

#define AC_MATH_OPERATION2(Type, name, category)                         \
    struct Type;                                                         \
                                                                         \
    struct Type##Scalar : operation_base<Type##Scalar> {                 \
        using parent = Type;                                             \
                                                                         \
        template<typename L, typename R>                                 \
        static constexpr auto evaluate(L l, R r) {                       \
            using std::name;                                             \
            return name(l, r);                                           \
        }                                                                \
    };                                                                   \
    inline constexpr Type##Scalar name##_scalar;                         \
                                                                         \
    struct Type : operation_base<Type> {                                 \
        using operation_category = category;                             \
                                                                         \
        template<typename L, typename R>                                 \
            requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) \
        friend constexpr auto specialization(                            \
            Type, type_array<L, R>                                       \
        ) noexcept {                                                     \
            return name##_scalar;                                        \
        }                                                                \
    };                                                                   \
    inline constexpr Type name;

} // namespace ac
