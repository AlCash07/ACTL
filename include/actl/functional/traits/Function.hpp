// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

// These includes aren't used in this file itself,
// but needed for all the specializations of function_traits.
#include <actl/functional/traits/function_category.hpp>
#include <actl/meta/type_list/type_list.hpp>
#include <actl/preprocessor/AC_IS_EMPTY.hpp>
#include <actl/preprocessor/AC_UNPARENTHESIZED.hpp>
#include <type_traits>

namespace ac {

template<class Fn>
struct function_traits;

template<class T>
concept Function = requires { function_traits<T>::category; };

template<class Fn>
struct function_traits<Fn&> : function_traits<Fn> {};
template<class Fn>
struct function_traits<Fn&&> : function_traits<Fn> {};

} // namespace ac
