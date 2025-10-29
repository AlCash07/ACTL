// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/container/conversion/between_ranges.hpp>
#include <actl/container/conversion/to_tuple.hpp>

namespace ac {

namespace detail {

template<typename S, typename To, typename From>
struct range_to_tuple_impl;

template<size_t... Is, typename To, typename From>
struct range_to_tuple_impl<std::index_sequence<Is...>, To, From> {
    using from_ref = range_reference_t<From>;

    static constexpr bool value =
        (static_size_v<From> == dynamic_size ||
         static_size_v<From> == std::tuple_size_v<To>) //
        &&(... && can_convert_to_v<std::tuple_element_t<Is, To>, from_ref>);

    static constexpr To convert(From&& x) noexcept(AC_ASSERT_IS_NOEXCEPT(
    ) && noexcept(To{convert_to<std::tuple_element_t<Is, To>>(x[Is])...})) {
        AC_ASSERT(std::tuple_size_v<To> == x.size());
        return To{convert_to<std::tuple_element_t<Is, To>>(x[Is])...};
    }
};

template<typename To, typename From>
using range_to_tuple = range_to_tuple_impl<tuple_indices_t<To>, To, From>;

template<typename To, typename From>
static constexpr bool range_to_tuple_test() {
    if constexpr (Tuple<To> && StrictRange<From>)
        return range_to_tuple<To, From>::value;
    return false;
}

} // namespace detail

template<typename To, typename From>
    requires(
        !can_convert_as_ranges<To, From>() &&
        detail::range_to_tuple_test<To, From>()
    )
struct conversion<To, From> : detail::range_to_tuple<To, From> {};

} // namespace ac
