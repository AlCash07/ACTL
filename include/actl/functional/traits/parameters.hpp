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

template<class Fn>
using return_t = typename function_traits<Fn>::return_type;

template<class Fn>
inline constexpr bool returns_void_v = std::is_same_v<return_t<Fn>, void>;

/// Parameter types of a function as ac::type_list.
///
/// @note Arguments name is sometimes incorrectly used instead of parameters.
/// Arguments are the values passed to the function when called,
/// so they are not a property of the function itself.
template<class Fn>
using parameters_t = typename function_traits<Fn>::parameters_type;

/// Arity of the function, that is its parameter count
/// excluding the variadic arguments.
template<class Fn>
inline constexpr size_t arity_v = parameters_t<Fn>::length;

template<class Fn, size_t Index>
using parameter_at_t = at_t<parameters_t<Fn>, Index>;

template<class Fn>
struct unique_parameters {
    using type = function_traits<Fn>::parameters_type;
};
template<class Fn>
    requires requires { typename function_traits<Fn>::unique_parameters_type; }
struct unique_parameters<Fn> {
    using type = function_traits<Fn>::unique_parameters_type;
};
/// Member functions with either empty or & qualifier
/// both take the class parameter by reference.
/// To disambiguate these cases, unique parameter types contain
/// class parameter with the same qualifiers as the member function,
/// without the implicitly added reference.
template<class Fn>
using unique_parameters_t = typename unique_parameters<Fn>::type;

} // namespace ac
