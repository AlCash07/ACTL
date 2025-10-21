// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/type_list/type_list.hpp>
#include <actl/preprocessor/AC_IS_EMPTY.hpp>

namespace ac::detail {

template<class Fn>
struct function_traits;

template<class Fn>
struct function_traits<Fn&> : function_traits<Fn> {};
template<class Fn>
struct function_traits<Fn&&> : function_traits<Fn> {};

} // namespace ac::detail
