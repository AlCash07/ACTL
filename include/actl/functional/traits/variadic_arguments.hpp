// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/FunctionObject.hpp>

namespace ac {

template<class Fn>
inline constexpr bool accepts_variadic_arguments_v =
    function_traits<Fn>::accepts_variadic_arguments;

} // namespace ac
