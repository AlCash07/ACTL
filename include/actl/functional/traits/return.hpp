// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/parameters.hpp>

namespace ac {

template<class Fn>
using return_t = typename function_traits<Fn>::return_type;

template<class Fn>
inline constexpr bool returns_void_v = std::is_same_v<return_t<Fn>, void>;

template<class Fn, class ReturnType>
struct with_return_type {
    using traits = function_traits<Fn>;
    using type = assemble_function_t<
        traits::category,
        ReturnType,
        unique_parameters_t<Fn>,
        traits::accepts_variadic_arguments,
        traits::is_noexcept>;
};
template<class... Ts, class ReturnType>
struct with_return_type<type_list<Ts...>, ReturnType> {
    using type = assemble_function_t<
        function_category::free,
        ReturnType,
        type_list<Ts...>,
        false,
        false>;
};
/// Resulting type is different for different input types T:
/// - If T is a function, then it's a function with the given return type,
///   but otherwise identical to T.
/// - If T is an ac::type_list template instantiaion, then it's a
///   free function with the given return type and T as its parameters list.
template<class T, class ReturnType>
using with_return_type_t = typename with_return_type<T, ReturnType>::type;

} // namespace ac
