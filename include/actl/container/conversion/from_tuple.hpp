// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/tuple.hpp>
#include <actl/container/conversion/convert_to.hpp>

namespace ac {

namespace detail {

template <size_t I, class Tuple>
constexpr decltype(auto) adl_get(Tuple const& x) noexcept
{
    using std::get;
    static_assert(noexcept(get<I>(x)));
    return get<I>(x);
}

template <
    class To,
    class From,
    class S = std::make_index_sequence<std::tuple_size_v<remove_cvref_t<From>>>>
struct from_tuple;

template <class To, class From, size_t... Is>
struct from_tuple<To, From, std::index_sequence<Is...>>
{
    static constexpr bool value =
        can_convert_to_v<To, decltype(adl_get<Is>(std::declval<From>()))...>;

    static constexpr To convert(From&& x) AC_DEDUCE_NOEXCEPT_AND_RETURN(
        convert_to<To>(adl_get<Is>(std::forward<From>(x))...))
};

} // namespace detail

template <class To, class From>
struct conversion_sfinae<
    std::enable_if_t<is_tuple_v<remove_cvref_t<From>>>,
    To,
    From> : detail::from_tuple<To, From>
{};

} // namespace ac
