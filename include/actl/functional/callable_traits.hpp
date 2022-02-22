// Traits to extract callable's return and parameter types.
// Reference: https://functionalcpp.wordpress.com/2013/08/05/function-traits/
// This is a lightweight analog of Boost.CallableTraits
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
struct callable_traits : function_object_traits<Function>
{};

template <class Function>
inline constexpr size_t arity_v = callable_traits<Function>::arity;

template <class Function>
using return_type_t = typename callable_traits<Function>::return_type;

template <size_t N, class Function>
using parameter_type_t =
    typename callable_traits<Function>::template parameter_type<N>;

template <class Function>
inline constexpr bool is_member_function_v =
    callable_traits<Function>::is_member;

template <class Function>
inline constexpr bool is_noexcept_v = callable_traits<Function>::is_noexcept;

// function references
template <class Function>
struct callable_traits<Function&> : callable_traits<Function>
{};

template <class Function>
struct callable_traits<Function&&> : callable_traits<Function>
{};

// free function
template <class Return, class... Params>
struct callable_traits<Return(Params...)>
{
    static constexpr size_t arity = sizeof...(Params);

    using return_type = Return;

    template <size_t N>
    using parameter_type = type_at_t<N, Params...>;

    static constexpr bool is_member = false;

    static constexpr bool is_noexcept = false;
};

template <class Return, class... Params>
struct callable_traits<Return(Params...) noexcept>
    : callable_traits<Return(Params...)>
{
    static constexpr bool is_noexcept = true;
};

// function pointer
template <class Return, class... Params>
struct callable_traits<Return (*)(Params...)>
    : callable_traits<Return(Params...)>
{};

template <class Return, class... Params>
struct callable_traits<Return (*)(Params...) noexcept>
    : callable_traits<Return(Params...) noexcept>
{};

// member function pointer
template <class Function>
struct member_function_traits : callable_traits<Function>
{
    static constexpr bool is_member = true;
};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...)>
    : member_function_traits<Return(Class&, Params...)>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...) noexcept>
    : member_function_traits<Return(Class&, Params...) noexcept>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...) const>
    : member_function_traits<Return(const Class&, Params...)>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...) const noexcept>
    : member_function_traits<Return(const Class&, Params...) noexcept>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...)&>
    : member_function_traits<Return(Class&, Params...)>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...)& noexcept>
    : member_function_traits<Return(Class&, Params...) noexcept>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...) const&>
    : member_function_traits<Return(const Class&, Params...)>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...) const& noexcept>
    : member_function_traits<Return(const Class&, Params...) noexcept>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...) &&>
    : member_function_traits<Return(Class&&, Params...)>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...)&& noexcept>
    : member_function_traits<Return(Class&&, Params...) noexcept>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...) const&&>
    : member_function_traits<Return(const Class&&, Params...)>
{};

template <class Class, class Return, class... Params>
struct callable_traits<Return (Class::*)(Params...) const&& noexcept>
    : member_function_traits<Return(const Class&&, Params...) noexcept>
{};

template <class Function>
struct function_object_traits<
    Function,
    std::void_t<decltype(&Function::operator())>>
{
private:
    using Operator = decltype(&Function::operator());

public:
    static constexpr size_t arity = arity_v<Operator> - 1;

    using return_type = return_type_t<Operator>;

    template <size_t N>
    using parameter_type = parameter_type_t<N + 1, Operator>;

    static constexpr bool is_member = false;

    static constexpr bool is_noexcept = is_noexcept_v<Operator>;
};

} // namespace ac
