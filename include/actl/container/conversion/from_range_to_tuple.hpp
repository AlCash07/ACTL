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

template<typename S, typename Target, typename Source>
struct range_to_tuple_impl;

template<size_t... Is, typename Target, typename Source>
struct range_to_tuple_impl<std::index_sequence<Is...>, Target, Source> {
    using source_ref = range_reference_t<Source>;

    static constexpr bool value =
        (static_size_v<Source> == dynamic_size ||
         static_size_v<Source> == std::tuple_size_v<Target>) //
        &&(... &&
           can_convert_to_v<std::tuple_element_t<Is, Target>, source_ref>);

    static constexpr Target convert(Source&& source
    ) noexcept(AC_ASSERT_IS_NOEXCEPT() && noexcept(Target{
        convert_to<std::tuple_element_t<Is, Target>>(source[Is])...
    })) {
        AC_ASSERT(std::tuple_size_v<Target> == source.size());
        return Target{convert_to<std::tuple_element_t<Is, Target>>(source[Is]
        )...};
    }
};

template<typename Target, typename Source>
using range_to_tuple =
    range_to_tuple_impl<tuple_indices_t<Target>, Target, Source>;

template<typename Target, typename Source>
static constexpr bool range_to_tuple_test() {
    if constexpr (Tuple<Target> && StrictRange<Source>)
        return range_to_tuple<Target, Source>::value;
    return false;
}

} // namespace detail

template<typename Target, typename Source>
    requires(
        !can_convert_as_ranges<Target, Source>() &&
        detail::range_to_tuple_test<Target, Source>()
    )
struct conversion<Target, Source> : detail::range_to_tuple<Target, Source> {};

} // namespace ac
