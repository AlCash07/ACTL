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

template<typename Target, typename Source, typename I = tuple_indices_t<Source>>
struct from_tuple;

template<typename Target, typename Source, size_t... Is>
struct from_tuple<Target, Source, std::index_sequence<Is...>> {
    static constexpr bool value = can_convert_to_v<
        Target,
        decltype(adl_get<Is>(std::declval<Source>()))...>;

    static constexpr Target convert(Source&& source)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(
            convert_to<Target>(adl_get<Is>(std::forward<Source>(source))...)
        )
};

} // namespace detail

template<typename Target, typename Source>
    requires(
        !can_convert_as_ranges<Target, Source>() && Tuple<Source> &&
        !StrictRange<Source> && (Tuple<Target> || Range<Target>)
    )
struct conversion<Target, Source> : detail::from_tuple<Target, Source> {};

} // namespace ac
