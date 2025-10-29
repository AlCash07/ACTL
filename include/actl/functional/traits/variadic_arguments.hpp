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
template<typename Fn>
inline constexpr bool accepts_variadic_arguments_v =
    function_traits<Fn>::accepts_variadic_arguments;

template<typename Fn>
struct add_variadic_arguments {
    using traits = function_traits<Fn>;
    using type = assemble_function_t<
        traits::category,
        typename traits::return_type,
        unique_parameters_t<Fn>,
        true,
        traits::is_noexcept>;
};
template<typename Fn>
using add_variadic_arguments_t = typename add_variadic_arguments<Fn>::type;

template<typename Fn>
struct remove_variadic_arguments {
    using traits = function_traits<Fn>;
    using type = assemble_function_t<
        traits::category,
        typename traits::return_type,
        unique_parameters_t<Fn>,
        false,
        traits::is_noexcept>;
};
template<typename Fn>
using remove_variadic_arguments_t =
    typename remove_variadic_arguments<Fn>::type;

} // namespace ac
