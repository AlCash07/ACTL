// User-defined literal _c that creates std::integral_constant values.
// Reference:
// https://blog.mattbierner.com/stupid-template-tricks-stdintegral_constant-user-defined-literal/
//
// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace ac {

namespace detail {

template <char C>
constexpr unsigned digit_to_value()
{
    if constexpr ('a' <= C && C <= 'f')
        return unsigned{C - 'a' + 10};
    else if constexpr ('A' <= C && C <= 'F')
        return unsigned{C - 'A' + 10};
    else if constexpr ('0' <= C && C <= '9')
        return unsigned{C - '0'};
}

template <class Digits, char... Cs>
struct filter_digits
{
    using type = Digits;
};

template <unsigned... Digits, char... Cs>
struct filter_digits<std::integer_sequence<unsigned, Digits...>, '\'', Cs...>
    : filter_digits<std::integer_sequence<unsigned, Digits...>, Cs...>
{};

template <unsigned... Digits, char C, char... Cs>
struct filter_digits<std::integer_sequence<unsigned, Digits...>, C, Cs...>
    : filter_digits<
          std::integer_sequence<unsigned, Digits..., digit_to_value<C>()>,
          Cs...>
{};

template <unsigned Base, char... Cs>
struct base_and_digits
{
    static constexpr unsigned base = Base;
    using digits =
        typename filter_digits<std::integer_sequence<unsigned>, Cs...>::type;
};

template <char... Cs>
struct to_number : base_and_digits<10, Cs...>
{};

template <char... Cs>
struct to_number<'0', 'X', Cs...> : base_and_digits<16, Cs...>
{};

template <char... Cs>
struct to_number<'0', 'x', Cs...> : base_and_digits<16, Cs...>
{};

template <char... Cs>
struct to_number<'0', Cs...> : base_and_digits<8, Cs...>
{};

template <char... Cs>
struct to_number<'0', 'b', Cs...> : base_and_digits<2, Cs...>
{};

template <char... Cs>
struct to_number<'0', 'B', Cs...> : base_and_digits<2, Cs...>
{};

template <
    class U,
    U X,
    bool Signed = X <= std::numeric_limits<std::make_signed_t<U>>::max()>
struct constant_impl
{
    using type = std::integral_constant<U, X>;
};

template <class U, U X>
struct constant_impl<U, X, true>
{
    using type = std::integral_constant<std::make_signed_t<U>, X>;
};

template <class U, char... Cs>
struct constant_from_str
{
    using number = to_number<Cs...>;

    template <unsigned D, unsigned... Ds>
    static constexpr U fold(U x, std::integer_sequence<unsigned, D, Ds...>)
    {
        static_assert(0 <= D && D < number::base);
        return fold(
            D + number::base * x, std::integer_sequence<unsigned, Ds...>{});
    }

    static constexpr U fold(U x, std::integer_sequence<unsigned>)
    {
        return x;
    }

    using unsigned_type =
        std::integral_constant<U, fold(U{0}, typename number::digits{})>;

    using type = typename constant_impl<U, unsigned_type::value>::type;
};

} // namespace detail

inline namespace constant_literals {

template <char... Cs>
constexpr auto operator""_c()
{
    return typename detail::constant_from_str<unsigned, Cs...>::type{};
}

template <char... Cs>
constexpr auto operator""_uc()
{
    return typename detail::constant_from_str<unsigned, Cs...>::unsigned_type{};
}

template <char... Cs>
constexpr auto operator""_cll()
{
    return
        typename detail::constant_from_str<unsigned long long, Cs...>::type{};
}

template <char... Cs>
constexpr auto operator""_ucll()
{
    return typename detail::constant_from_str<unsigned long long, Cs...>::
        unsigned_type{};
}

} // namespace constant_literals

} // namespace ac
