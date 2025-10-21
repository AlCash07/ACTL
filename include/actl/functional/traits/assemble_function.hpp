// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/Function.hpp>

namespace ac {

template<
    function_category Category,
    class Return,
    class ParametersList,
    bool AcceptsVArgs,
    bool IsNoexcept>
struct assemble_function;

template<
    function_category Category,
    class Return,
    class ParametersList,
    bool AcceptsVArgs,
    bool IsNoexcept>
using assemble_function_t = typename assemble_function<
    Category,
    Return,
    ParametersList,
    AcceptsVArgs,
    IsNoexcept>::type;

/// Member function with either empty or & qualifier
/// results in the class parameter being passed by reference.
/// Because of that, we need to use unique types when assembling a function.
template<class Fn>
using unique_parameters_t = decltype([] {
    if constexpr (requires {
                      typename function_traits<Fn>::unique_parameter_types;
                  })
        return typename function_traits<Fn>::unique_parameter_types{};
    else
        return typename function_traits<Fn>::parameter_types{};
}());

} // namespace ac
