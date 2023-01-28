// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/meta/can_list_initialize.hpp>
#include <actl/meta/tuple.hpp>

namespace ac {

template <class To, class... Args>
struct conversion_default : std::true_type
{
    static constexpr To convert(Args&&... xs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(To{std::forward<Args>(xs)...})
};

template <class AlwaysVoid, class To, class... Args>
struct conversion_sfinae : std::false_type
{};

template <class To, class From>
struct conversion_sfinae<
    std::enable_if_t<
        std::is_arithmetic_v<To> && std::is_convertible_v<From, To>>,
    To,
    From> : std::true_type
{
    static constexpr To convert(From&& x)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(static_cast<To>(std::forward<From>(x)))
};

// List initialization is intentionally used here instead of construction
// to avoid accidental calls to constructors such as std::vector(size_t n).
// Conversion to a tuple can't rely on its constructors (except for a default
// one), because they commonly accept any number of arguments not greater than
// the tuple size. We constrain conversion to exactly the tuple size to avoid
// unexpected behavior when some of the arguments are not specified by mistake.
// To convert to a tuple (including an array), include to_tuple.hpp.
template <class To, class... Args>
struct conversion
    : std::conditional_t<
          (sizeof...(Args) == 0 || !Tuple<To>)&& //
          can_list_initialize_v<To, Args...>,
          conversion_default<To, Args...>,
          conversion_sfinae<void, To, Args...>>
{};

template <class To, class... Args>
inline constexpr bool can_convert_to_v = conversion<To, Args...>::value;

template <class To, class... Args>
constexpr To convert_to(Args&&... xs) AC_DEDUCE_NOEXCEPT_AND_RETURN(
    conversion<To, Args...>::convert(std::forward<Args>(xs)...))

} // namespace ac
