// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/category/member_function.hpp>
#include <actl/functional/traits/detail/free_function.hpp>

namespace ac::detail {

template<class Fn>
    requires MemberFunction<std::remove_cvref_t<Fn>>
struct function_traits<Fn>
    : function_traits<detail::as_free_function_t<std::remove_cvref_t<Fn>>> {};

} // namespace ac::detail
