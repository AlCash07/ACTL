// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/FreeFunction.hpp>
#include <actl/meta/type_at.hpp>

namespace ac::detail {

template<class Fn>
struct function_traits {};

template<class Return, class... Parameters>
struct function_traits<Return(Parameters...)> {
    using return_type = Return;

    static constexpr size_t arity = sizeof...(Parameters);

    template<size_t Index>
    using parameter_at = type_at_t<Index, Parameters...>;

    static constexpr bool accepts_variadic_arguments = false;

    static constexpr bool is_noexcept = false;
};

template<class Return, class... Parameters>
struct function_traits<Return(Parameters...) noexcept>
    : function_traits<Return(Parameters...)> {
    static constexpr bool is_noexcept = true;
};

template<class Return, class... Parameters>
struct function_traits<Return(Parameters..., ...)>
    : function_traits<Return(Parameters...)> {
    static constexpr bool accepts_variadic_arguments = true;
};

template<class Return, class... Parameters>
struct function_traits<Return(Parameters..., ...) noexcept>
    : function_traits<Return(Parameters...)> {
    static constexpr bool accepts_variadic_arguments = true;

    static constexpr bool is_noexcept = true;
};

// Type qualifiers and pointer don't matter for regular functions.
template<class Fn>
    requires FreeFunction<std::remove_pointer_t<std::remove_cvref_t<Fn>>>
struct function_traits<Fn>
    : function_traits<std::remove_pointer_t<std::remove_cvref_t<Fn>>> {};

} // namespace ac::detail
