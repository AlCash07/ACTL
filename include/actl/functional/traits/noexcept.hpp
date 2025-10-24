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
template<class Fn>
inline constexpr bool is_noexcept_v = function_traits<Fn>::is_noexcept;

template<class Fn, class Traits = function_traits<Fn>>
using add_noexcept_t = assemble_function_t<
    Traits::category,
    typename Traits::return_type,
    unique_parameters_t<Fn>,
    Traits::accepts_variadic_arguments,
    true>;

template<class Fn, class Traits = function_traits<Fn>>
using remove_noexcept_t = assemble_function_t<
    Traits::category,
    typename Traits::return_type,
    unique_parameters_t<Fn>,
    Traits::accepts_variadic_arguments,
    false>;

} // namespace ac
