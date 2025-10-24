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
template<class Fn>
inline constexpr bool accepts_variadic_arguments_v =
    function_traits<Fn>::accepts_variadic_arguments;

template<class Fn, class Traits = function_traits<Fn>>
using add_variadic_arguments_t = assemble_function_t<
    Traits::category,
    typename Traits::return_type,
    unique_parameters_t<Fn>,
    true,
    Traits::is_noexcept>;

template<class Fn, class Traits = function_traits<Fn>>
using remove_variadic_arguments_t = assemble_function_t<
    Traits::category,
    typename Traits::return_type,
    unique_parameters_t<Fn>,
    false,
    Traits::is_noexcept>;

} // namespace ac
