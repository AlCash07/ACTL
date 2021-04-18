// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/scalar.hpp>
#include <actl/category/utility/is_subcategory_of.hpp>
#include <actl/meta/strict_common_type.hpp>
#include <actl/operation/operation.hpp>
#include <actl/operation/scalar/enable_operators.hpp>
#include <actl/operation/scalar/scalar_expression.hpp>

namespace ac {

// clang-format off
struct scalar_operation_tag { using base = operation_tag; };

struct arithmetic_operation_tag     { using base = scalar_operation_tag; };
struct additive_operation_tag       { using base = arithmetic_operation_tag; };
struct multiplicative_operation_tag { using base = arithmetic_operation_tag; };

struct bit_operation_tag     { using base = scalar_operation_tag; };
struct bitwise_operation_tag { using base = bit_operation_tag; };

struct comparison_operation_tag { using base = scalar_operation_tag; };
struct equality_operation_tag   { using base = comparison_operation_tag; };
struct ordering_operation_tag   { using base = comparison_operation_tag; };

struct logical_operation_tag { using base = scalar_operation_tag; };
// clang-format on

template <class Op, index Arity>
struct scalar_operation : operation<Op> {
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
        if constexpr ((... &&
                       is_subcategory_of_v<category_t<Ts>, arithmetic_tag>)) {
            using T = strict_common_type_t<decltype(eval(xs))...>;
            if constexpr (!is_integral_constant_v<T>)
                return this->derived().eval_scalar(convert<T>(eval(xs))...);
        }
        return this->derived().eval_scalar(eval(xs)...);
    }

    template <class T, class... Ts>
    constexpr void evaluate_to(T& dst, const Ts&... xs) const {
        dst = evaluate(xs...);
    }
};

template <class T>
constexpr bool is_scalar_operation_v =
    is_subcategory_of_v<category_t<T>, scalar_operation_tag>;

template <class T>
constexpr bool is_comparison_operation_v =
    is_subcategory_of_v<category_t<T>, comparison_operation_tag>;

} // namespace ac
