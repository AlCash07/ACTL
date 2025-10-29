// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/function_traits.hpp>

namespace ac {

template<
    function_category Category,
    typename Return,
    typename ParametersList,
    bool AcceptsVArgs,
    bool IsNoexcept>
struct assemble_function;

/// Assembles a function type with the given properties.
template<
    function_category Category,
    typename Return,
    typename ParametersList,
    bool AcceptsVArgs,
    bool IsNoexcept>
    requires(Category != function_category::object)
using assemble_function_t = typename assemble_function<
    Category,
    Return,
    ParametersList,
    AcceptsVArgs,
    IsNoexcept>::type;

template<typename Fn>
struct as_free_function {
    using traits = function_traits<Fn>;
    using type = assemble_function_t<
        function_category::free,
        typename traits::return_type,
        typename traits::parameters_type,
        traits::accepts_variadic_arguments,
        traits::is_noexcept>;
};
/// Free function with the same parameters and return type as Fn.
template<typename Fn>
using as_free_function_t = typename as_free_function<Fn>::type;

} // namespace ac
