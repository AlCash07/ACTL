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
template<typename Fn>
inline constexpr bool is_noexcept_v = function_traits<Fn>::is_noexcept;

template<typename Fn>
struct add_noexcept {
    using traits = function_traits<Fn>;
    using type = assemble_function_t<
        traits::category,
        typename traits::return_type,
        unique_parameters_t<Fn>,
        traits::accepts_variadic_arguments,
        true>;
};
template<typename Fn>
using add_noexcept_t = typename add_noexcept<Fn>::type;

template<typename Fn>
struct remove_noexcept {
    using traits = function_traits<Fn>;
    using type = assemble_function_t<
        traits::category,
        typename traits::return_type,
        unique_parameters_t<Fn>,
        traits::accepts_variadic_arguments,
        false>;
};
template<typename Fn>
using remove_noexcept_t = typename remove_noexcept<Fn>::type;

} // namespace ac
