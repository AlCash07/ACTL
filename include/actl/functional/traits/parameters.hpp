// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/FreeFunction.hpp>
#include <actl/functional/traits/FunctionObject.hpp>
#include <actl/meta/type_list/at.hpp>

namespace ac {

/// Parameter types of a function as ac::type_list.
///
/// @note Arguments name is sometimes incorrectly used instead of parameters.
/// Arguments are the values passed to the function when called,
/// so they are not a property of the function itself.
template<typename Function>
using parameters_t = typename function_traits<Function>::parameters_type;

/// Arity of the function, that is its parameter count
/// excluding the variadic arguments.
template<typename Function>
inline constexpr size_t arity_v = parameters_t<Function>::length;

template<typename Function, size_t Index>
using parameter_at_t = at_t<parameters_t<Function>, Index>;

template<typename Function>
struct unique_parameters {
    using type = function_traits<Function>::parameters_type;
};
template<typename Function>
    requires requires {
        typename function_traits<Function>::unique_parameters_type;
    }
struct unique_parameters<Function> {
    using type = function_traits<Function>::unique_parameters_type;
};
/// Member functions with either empty or & qualifier
/// both take the class parameter by reference.
/// To disambiguate these cases, unique parameter types contain
/// class parameter with the same qualifiers as the member function,
/// without the implicitly added reference.
template<typename Function>
using unique_parameters_t = typename unique_parameters<Function>::type;

} // namespace ac
