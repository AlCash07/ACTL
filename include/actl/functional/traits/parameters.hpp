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
using parameters_t = typename function_traits<Fn>::parameter_types;

template<class Fn>
inline constexpr size_t arity_v = parameters_t<Fn>::length;

template<size_t Index, class Fn>
using parameter_at_t = at_t<parameters_t<Fn>, Index>;

} // namespace ac
