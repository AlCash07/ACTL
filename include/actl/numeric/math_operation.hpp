// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/scalar_operation.hpp>
#include <cmath>

namespace ac {

#define AC_MATH_OPERATION1(Type, name, category)                     \
    struct Type : operation_base<Type> {                             \
        using operation_category = category;                         \
    };                                                               \
    inline constexpr Type name;                                      \
                                                                     \
    struct Type##Scalar : operation_base<Type##Scalar> {             \
        using parent = Type;                                         \
                                                                     \
        template<typename T>                                         \
        static constexpr bool requirement = std::is_arithmetic_v<T>; \
                                                                     \
        template<typename T>                                         \
        static constexpr auto evaluate(T t) {                        \
            using std::name;                                         \
            return name(t);                                          \
        }                                                            \
    };                                                               \
    AC_REGISTER_SPECIALIZATION(Type##Scalar)                         \
    inline constexpr Type##Scalar name##_scalar;

#define AC_MATH_OPERATION2(Type, name, category)                \
    struct Type : operation_base<Type> {                        \
        using operation_category = category;                    \
    };                                                          \
    inline constexpr Type name;                                 \
                                                                \
    struct Type##Scalar : operation_base<Type##Scalar> {        \
        using parent = Type;                                    \
                                                                \
        template<typename L, typename R>                        \
        static constexpr bool requirement =                     \
            std::is_arithmetic_v<L> && std::is_arithmetic_v<R>; \
                                                                \
        template<typename L, typename R>                        \
        static constexpr auto evaluate(L l, R r) {              \
            using std::name;                                    \
            return name(l, r);                                  \
        }                                                       \
    };                                                          \
    AC_REGISTER_SPECIALIZATION(Type##Scalar)                    \
    inline constexpr Type##Scalar name##_scalar;

} // namespace ac
