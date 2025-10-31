// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/FreeFunction.hpp>
#include <actl/functional/traits/FunctionObject.hpp>

namespace ac {

/// Checks if the function is noexcept, see
/// https://en.cppreference.com/w/cpp/language/noexcept_spec.html
template<typename Function>
inline constexpr bool is_noexcept_v = function_traits<Function>::is_noexcept;

template<typename Function>
struct add_noexcept {
    using traits = function_traits<Function>;
    using type = assemble_function_t<
        traits::category,
        typename traits::return_type,
        unique_parameters_t<Function>,
        traits::accepts_variadic_arguments,
        true>;
};
template<typename Function>
using add_noexcept_t = typename add_noexcept<Function>::type;

template<typename Function>
struct remove_noexcept {
    using traits = function_traits<Function>;
    using type = assemble_function_t<
        traits::category,
        typename traits::return_type,
        unique_parameters_t<Function>,
        traits::accepts_variadic_arguments,
        false>;
};
template<typename Function>
using remove_noexcept_t = typename remove_noexcept<Function>::type;

} // namespace ac
