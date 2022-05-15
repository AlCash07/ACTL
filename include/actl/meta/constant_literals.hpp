// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/constant.hpp>
#include <limits>
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

template <class T, char... Cs>
struct str_to_constant
{
    using U = std::make_unsigned_t<T>;
    using number = to_number<Cs...>;

    template <U X, unsigned D, unsigned... Ds>
    static constexpr U fold(std::integer_sequence<unsigned, D, Ds...>)
    {
        static_assert(0 <= D && D < number::base);
        static_assert(X <= (std::numeric_limits<U>::max() - D) / number::base);
        return fold<D + number::base * X>(
            std::integer_sequence<unsigned, Ds...>{});
    }

    template <U X>
    static constexpr U fold(std::integer_sequence<unsigned>)
    {
        return X;
    }

    static constexpr auto value =
        constant<T{fold<0>(typename number::digits{})}>{};
};

} // namespace detail

inline namespace constant_literals {

/// Literal that produces ac::constant of type `int`.
template <char... Chars>
constexpr auto operator""_c()
{
    return detail::str_to_constant<int, Chars...>::value;
}

/// Literal that produces ac::constant of type `unsigned`.
template <char... Chars>
constexpr auto operator""_cu()
{
    return detail::str_to_constant<unsigned, Chars...>::value;
}

/// Literal that produces ac::constant of type `long long`.
template <char... Chars>
constexpr auto operator""_cll()
{
    return detail::str_to_constant<long long, Chars...>::value;
}

/// Literal that produces ac::constant of type `unsigned long long`.
template <char... Chars>
constexpr auto operator""_cull()
{
    return detail::str_to_constant<unsigned long long, Chars...>::value;
}

} // namespace constant_literals

} // namespace ac
