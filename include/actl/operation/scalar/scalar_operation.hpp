// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/strict_common_type.hpp>
#include <actl/operation/operation.hpp>
#include <actl/operation/scalar/enable_operators.hpp>
#include <actl/operation/scalar/scalar_expression.hpp>

namespace ac {

// clang-format off
struct scalar_operation_tag : operation_tag {};

struct arithmetic_operation_tag     : scalar_operation_tag {};
struct additive_operation_tag       : arithmetic_operation_tag {};
struct multiplicative_operation_tag : arithmetic_operation_tag {};

struct bit_operation_tag     : scalar_operation_tag {};
struct bitwise_operation_tag : bit_operation_tag {};

struct comparison_operation_tag : scalar_operation_tag {};
struct equality_operation_tag   : comparison_operation_tag {};
struct ordering_operation_tag   : comparison_operation_tag {};

struct logical_operation_tag : scalar_operation_tag {};
// clang-format on

template<class Op, size_t Arity>
struct scalar_operation : operation<Op> {
    template<class T>
    static constexpr T convert(T x) {
        return x;
    }

    template<class T, auto X>
    static constexpr T convert(constant<X>) {
        return T{X};
    }

    template<class... Ts>
    constexpr auto evaluate(Ts const&... xs) const {
        if constexpr ((... && std::is_arithmetic_v<
                                  unwrap_constant_t<decltype(eval(xs))>>)) {
            using T = strict_common_type_t<decltype(eval(xs))...>;
            if constexpr (!is_constant_v<T>)
                return this->derived().eval_scalar(convert<T>(eval(xs))...);
        } else {
            return this->derived().eval_scalar(eval(xs)...);
        }
    }

    template<class T, class... Ts>
    constexpr void evaluate_to(T& dst, Ts const&... xs) const {
        dst = evaluate(xs...);
    }
};

template<class T>
concept ScalarOperation =
    Operation<T> &&
    std::derived_from<typename T::operation_category, scalar_operation_tag>;

template<class T>
concept ComparisonOperation =
    ScalarOperation<T> &&
    std::derived_from<typename T::operation_category, comparison_operation_tag>;

} // namespace ac
