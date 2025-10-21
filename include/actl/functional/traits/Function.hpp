// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

// These includes aren't used in this file itself,
// but are needed for all the specializations of function_traits.
#include <actl/functional/traits/function_category.hpp>
#include <actl/meta/type_list/type_list.hpp>
#include <actl/preprocessor/AC_IS_EMPTY.hpp>
#include <actl/preprocessor/AC_UNPARENTHESIZED.hpp>
#include <type_traits>

namespace ac {

/// If @p Fn is a function, then specifies the following traits:
/// - category as a function_category;
/// - return_type;
/// - parameter_types as a type_list;
/// - accepts_variadic_arguments as a bool;
/// - is_noexcept as a bool.
template<class Fn>
struct function_traits;

/// Concept of a function, that is a type that:
/// - supports function call syntax:
///   function_name(arguments);
/// - has unique return and parameter types,
///   which excludes overload sets.
template<class T>
concept Function = requires { function_traits<T>::category; };

/* Implementation */

// Reference qualifier doesn't matter for a function.
template<Function Fn>
struct function_traits<Fn&> : function_traits<Fn> {};
template<Function Fn>
struct function_traits<Fn&&> : function_traits<Fn> {};

} // namespace ac
