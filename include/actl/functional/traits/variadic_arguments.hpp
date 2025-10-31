// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/FreeFunction.hpp>
#include <actl/functional/traits/FunctionObject.hpp>

namespace ac {

/// Checks if the function accepts variadic arguments, see
/// https://en.cppreference.com/w/cpp/language/variadic_arguments.html
template<typename Function>
inline constexpr bool accepts_variadic_arguments_v =
    function_traits<Function>::accepts_variadic_arguments;

template<typename Function>
struct add_variadic_arguments {
    using traits = function_traits<Function>;
    using type = assemble_function_t<
        traits::category,
        typename traits::return_type,
        unique_parameters_t<Function>,
        true,
        traits::is_noexcept>;
};
template<typename Function>
using add_variadic_arguments_t =
    typename add_variadic_arguments<Function>::type;

template<typename Function>
struct remove_variadic_arguments {
    using traits = function_traits<Function>;
    using type = assemble_function_t<
        traits::category,
        typename traits::return_type,
        unique_parameters_t<Function>,
        false,
        traits::is_noexcept>;
};
template<typename Function>
using remove_variadic_arguments_t =
    typename remove_variadic_arguments<Function>::type;

} // namespace ac
