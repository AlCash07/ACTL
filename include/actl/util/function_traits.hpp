/***************************************************************************************************
 * Traits to extract callable object's argument and return types.
 * Reference: https://functionalcpp.wordpress.com/2013/08/05/function-traits/
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/type_traits.hpp>

namespace ac {

template <class F, class = void>
struct functor_traits {};

template <class F>
struct function_traits : functor_traits<F> {};

template <class F>
inline constexpr size_t arity_v = function_traits<F>::arity;

template <class F>
using return_type_t = typename function_traits<F>::return_type;

template <class F, size_t N>
using argument_type_t = typename function_traits<F>::template argument_type<N>;

// function pointer
template <class R, class... Ts>
struct function_traits<R (*)(Ts...)> : function_traits<R(Ts...)> {};

// free function
template <class R, class... Ts>
struct function_traits<R(Ts...)> {
    static constexpr size_t arity = sizeof...(Ts);

    using return_type = R;

    template <size_t N>
    using argument_type = nth_t<N, Ts...>;
};

// member function pointer
template <class C, class R, class... Args>
struct function_traits<R (C::*)(Args...)> : function_traits<R(C&, Args...)> {};

// const member function pointer
template <class C, class R, class... Args>
struct function_traits<R (C::*)(Args...) const> : function_traits<R(const C&, Args...)> {};

// functor object
template <class F>
struct functor_traits<F, std::void_t<decltype(&F::operator())>> {
private:
    using O = decltype(&F::operator());

public:
    static constexpr size_t arity = arity_v<O> - 1;

    using return_type = return_type_t<O>;

    template <size_t N>
    using argument_type = argument_type_t<O, N + 1>;
};

}  // namespace ac
