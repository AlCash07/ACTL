// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/between_ranges.hpp>
#include <actl/container/conversion/convert_to.hpp>
#include <actl/meta/tuple.hpp>

namespace ac {

namespace detail {

template<size_t I>
constexpr decltype(auto) adl_get(Tuple auto const& x) noexcept {
    using std::get;
    static_assert(noexcept(get<I>(x)));
    return get<I>(x);
}

template<typename To, typename From, typename S = tuple_indices_t<From>>
struct from_tuple;

template<typename To, typename From, size_t... Is>
struct from_tuple<To, From, std::index_sequence<Is...>> {
    static constexpr bool value =
        can_convert_to_v<To, decltype(adl_get<Is>(std::declval<From>()))...>;

    static constexpr To convert(From&& x) AC_DEDUCE_NOEXCEPT_AND_RETURN(
        convert_to<To>(adl_get<Is>(std::forward<From>(x))...)
    )
};

} // namespace detail

template<typename To, typename From>
    requires(
        !can_convert_as_ranges<To, From>() && Tuple<From> &&
        !StrictRange<From> && (Tuple<To> || Range<To>)
    )
struct conversion<To, From> : detail::from_tuple<To, From> {};

} // namespace ac
