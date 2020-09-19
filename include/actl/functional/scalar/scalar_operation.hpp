/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation/operation.hpp>
#include <actl/functional/scalar/argument_tags.hpp>

namespace ac::math {

template <class Op, index Arity, class ArgumentsTag>
struct scalar_operation : operation<Op> {
    struct is_scalar_operation;

    template <class... Ts>
    constexpr auto evaluate(const Ts&... xs) const {
        if constexpr ((... && std::is_base_of_v<arithmetic_tag, category_t<Ts>>)) {
            static_assert(are_same_v<remove_cvref_t<decltype(eval(xs))>...> &&
                          (... && std::is_base_of_v<ArgumentsTag, category_t<Ts>>));
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
inline constexpr bool is_scalar_operation_v = is_scalar_operation<T>::value;

}  // namespace ac::math
