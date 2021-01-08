// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/operation.hpp>
#include <actl/functional/scalar/enable_operators.hpp>
#include <actl/traits/category/scalar.hpp>
#include <actl/traits/strict_common_type.hpp>

namespace ac {

template <class Op, index Arity, class ArgumentsTag>
struct scalar_operation : operation<Op> {
    struct is_scalar_operation;

    template <class T>
    static constexpr T convert(T x) {
        return x;
    }

    template <class T, class U, U X>
    static constexpr T convert(std::integral_constant<U, X>) {
        return T{X};
    }

    template <class... Ts>
    constexpr auto evaluate(const Ts&... xs) const {
        if constexpr ((... && std::is_base_of_v<arithmetic_tag, category_t<Ts>>)) {
            using T = strict_common_type_t<decltype(eval(xs))...>;
            if constexpr (!is_integral_constant_v<T>) {
                static_assert(std::is_base_of_v<ArgumentsTag, category_t<T>>);
                return this->derived().eval_scalar(convert<T>(eval(xs))...);
            }
        }
        return this->derived().eval_scalar(eval(xs)...);
    }

    template <class T, class... Ts>
    constexpr void evaluate_to(T& dst, const Ts&... xs) const {
        dst = evaluate(xs...);
    }
};

template <class T, class = void>
struct is_scalar_operation : std::false_type {};

template <class T>
struct is_scalar_operation<T, std::void_t<typename T::is_scalar_operation>> : std::true_type {};

template <class T>
constexpr bool is_scalar_operation_v = is_scalar_operation<T>::value;

}  // namespace ac
