// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/convert_to.hpp>
#include <actl/meta/can_list_initialize.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <actl/range/traits/strict_range.hpp>

namespace ac {

template<typename Target, typename... Args>
struct range_initialization : std::true_type {
    // List initialization is intentionally used here instead of construction
    // to avoid accidental calls to constructors such as std::vector(size_t n).
    static constexpr Target convert(Args&&... args
    ) AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        Target{ac::convert_to<range_value_t<Target>>(std::forward<Args>(args)
        )...}
    )
};

template<typename T, typename>
using repeat_t = T;

template<typename Target, typename... Args>
constexpr bool can_initialize_range() noexcept {
    // We check for StrictRange, because we don't want to
    // miss additional type checking enabled by the tuple.
    if constexpr (StrictRange<Target>)
        return static_sizes_match(static_size_v<Target>, sizeof...(Args)) &&
               (... && can_convert_to_v<range_value_t<Target>, Args>)&& //
               can_list_initialize_v<
                   Target,
                   repeat_t<range_value_t<Target>, Args>...>;
    else
        return false;
}

template<typename Target, typename... Args>
    requires(can_initialize_range<Target, Args...>())
struct conversion<Target, Args...> : range_initialization<Target, Args...> {};

} // namespace ac
