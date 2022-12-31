// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/member_function_traits.hpp>

namespace ac {

/// Callable traits specialized only for function objects.
template <class T>
struct function_object_traits
{
    static constexpr bool is_function_object = false;
};

/// Indicates whether a type is a function function object,
/// that is has a non-overloaded `operator()`.
/// Type qualifiers are taken into account.
template <class T>
inline constexpr bool is_function_object_v =
    function_object_traits<T>::is_function_object;

template <class Fn>
requires requires(Fn)
{
    &Fn::operator();
}
struct function_object_traits<Fn>
{
private:
    using Traits = member_function_traits<decltype(&Fn::operator())>;

public:
    using return_type = typename Traits::return_type;

    static constexpr size_t arity = Traits::arity - 1;

    template <size_t Index>
    using parameter_type = typename Traits::template parameter_type<Index + 1>;

    static constexpr bool is_noexcept = Traits::is_noexcept;

    static constexpr bool is_function_object = true;
};

} // namespace ac
