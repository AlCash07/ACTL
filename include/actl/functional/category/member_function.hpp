// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/category/free_function.hpp>

namespace ac {

/// Callable traits specialized only for member function pointers.
template<class T>
struct member_function_traits {
    static constexpr bool is_member_function = false;
};

template<class T>
concept MemberFunction = member_function_traits<T>::is_member_function;

namespace detail {

template<class Fn>
struct member_as_free_fn : free_function_traits<Fn> {
    static constexpr bool is_member_function = true;
};

} // namespace detail

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...)>
    : detail::member_as_free_fn<Return(Class&, Params...)> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) noexcept>
    : detail::member_as_free_fn<Return(Class&, Params...) noexcept> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) const>
    : detail::member_as_free_fn<Return(Class const&, Params...)> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) const noexcept>
    : detail::member_as_free_fn<Return(Class const&, Params...) noexcept> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...)&>
    : detail::member_as_free_fn<Return(Class&, Params...)> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) & noexcept>
    : detail::member_as_free_fn<Return(Class&, Params...) noexcept> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) const&>
    : detail::member_as_free_fn<Return(Class const&, Params...)> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) const & noexcept>
    : detail::member_as_free_fn<Return(Class const&, Params...) noexcept> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) &&>
    : detail::member_as_free_fn<Return(Class&&, Params...)> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) && noexcept>
    : detail::member_as_free_fn<Return(Class&&, Params...) noexcept> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) const&&>
    : detail::member_as_free_fn<Return(Class const&&, Params...)> {};

template<class Class, class Return, class... Params>
struct member_function_traits<Return (Class::*)(Params...) const && noexcept>
    : detail::member_as_free_fn<Return(Class const&&, Params...) noexcept> {};

} // namespace ac
