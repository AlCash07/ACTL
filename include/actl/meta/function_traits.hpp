// Traits to extract callable object's argument and return types.
// Reference: https://functionalcpp.wordpress.com/2013/08/05/function-traits/
//
// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/type_at.hpp>
#include <type_traits>

namespace ac {

template <class Function, class = void>
struct function_object_traits
{};

template <class Function>
struct function_traits
    : function_object_traits<std::remove_reference_t<Function>>
{};

template <class Function>
inline constexpr size_t arity_v = function_traits<Function>::arity;

template <class Function>
using return_type_t = typename function_traits<Function>::return_type;

template <size_t N, class Function>
using parameter_type_t =
    typename function_traits<Function>::template parameter_type<N>;

template <class Function>
inline constexpr bool is_noexcept_v = function_traits<Function>::is_noexcept;

// free function
template <class Return, class... Params>
struct function_traits<Return(Params...)>
{
    static constexpr size_t arity = sizeof...(Params);

    using return_type = Return;

    template <size_t N>
    using parameter_type = type_at_t<N, Params...>;

    static constexpr bool is_noexcept = false;
};

template <class Return, class... Params>
struct function_traits<Return(Params...) noexcept>
    : function_traits<Return(Params...)>
{
    static constexpr bool is_noexcept = true;
};

// function pointer
template <class Return, class... Params>
struct function_traits<Return (*)(Params...)>
    : function_traits<Return(Params...)>
{};

template <class Return, class... Params>
struct function_traits<Return (*)(Params...) noexcept>
    : function_traits<Return(Params...) noexcept>
{};

// member function pointer
template <class Class, class Return, class... Params>
struct function_traits<Return (Class::*)(Params...)>
    : function_traits<Return(Class&, Params...)>
{};

template <class Class, class Return, class... Params>
struct function_traits<Return (Class::*)(Params...) noexcept>
    : function_traits<Return(Class&, Params...) noexcept>
{};

template <class Class, class Return, class... Params>
struct function_traits<Return (Class::*)(Params...) const>
    : function_traits<Return(const Class&, Params...)>
{};

template <class Class, class Return, class... Params>
struct function_traits<Return (Class::*)(Params...) const noexcept>
    : function_traits<Return(const Class&, Params...) noexcept>
{};

template <class Function>
struct function_object_traits<
    Function,
    std::void_t<decltype(&Function::operator())>>
{
private:
    using O = decltype(&Function::operator());

public:
    static constexpr size_t arity = arity_v<O> - 1;

    using return_type = return_type_t<O>;

    template <size_t N>
    using parameter_type = parameter_type_t<N + 1, O>;

    static constexpr bool is_noexcept = is_noexcept_v<O>;
};

} // namespace ac
