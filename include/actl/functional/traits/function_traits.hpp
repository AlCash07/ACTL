// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

// These includes aren't used in this file itself,
// but are needed for all the specializations of function_traits.
#include <actl/meta/type_list/type_list.hpp>
#include <actl/preprocessor/AC_IS_EMPTY.hpp>
#include <actl/preprocessor/AC_UNPARENTHESIZED.hpp>
#include <type_traits>

namespace ac {

enum class function_category { free, member, object };

/// If @p T is a function, then the following members are specified:
/// - category as ac::function_category;
/// - return_type;
/// - parameters_type as ac::type_list;
/// - accepts_variadic_arguments as a `bool`;
/// - is_noexcept as a `bool`.
template<typename T>
struct function_traits;

// Reference qualifier doesn't matter for a function.
template<typename Function>
    requires requires { function_traits<Function>::category; }
struct function_traits<Function&> : function_traits<Function> {};
template<typename Function>
    requires requires { function_traits<Function>::category; }
struct function_traits<Function&&> : function_traits<Function> {};

} // namespace ac
