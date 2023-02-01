// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/deduce_noexcept_and_return.hpp>
#include <type_traits>
#include <utility> // for std::forward

namespace ac {

template <class To, class... Args>
struct conversion : std::false_type
{};

template <class To>
    requires(std::is_default_constructible_v<To>)
struct conversion<To> : std::true_type
{
    static constexpr To convert() AC_DEDUCE_NOEXCEPT_AND_RETURN(To{})
};

template <class To, class From>
    requires(std::is_arithmetic_v<To> && std::is_convertible_v<From, To>)
struct conversion<To, From> : std::true_type
{
    static constexpr To convert(From&& x)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(static_cast<To>(std::forward<From>(x)))
};

template <class To, class... Args>
inline constexpr bool can_convert_to_v = conversion<To, Args...>::value;

template <class To, class... Args>
constexpr To convert_to(Args&&... xs) AC_DEDUCE_NOEXCEPT_AND_RETURN(
    conversion<To, Args...>::convert(std::forward<Args>(xs)...))

} // namespace ac
