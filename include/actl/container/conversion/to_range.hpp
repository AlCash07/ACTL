// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/convert_to.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <actl/range/traits/is_strict_range.hpp>

namespace ac {

template <class To, class... From>
struct range_initialization : std::true_type
{
    static constexpr To convert(From&&... xs) AC_DEDUCE_NOEXCEPT_AND_RETURN(To{
        ac::convert_to<range_value_t<To>>(std::forward<From>(xs))...})
};

template <class T, class>
using repeat_t = T;

template <class To, class... From>
constexpr bool can_initialize_range() noexcept
{
    // We check for is_strict_range_v, because we don't want to
    // miss additional type checking enabled by the tuple.
    if constexpr (is_strict_range_v<To>)
        return static_sizes_match(static_size_v<To>, sizeof...(From)) &&
               (... && can_convert_to_v<range_value_t<To>, From>)&& //
               can_list_initialize_v<To, repeat_t<range_value_t<To>, From>...>;
    else
        return false;
}

template <class To, class... From>
struct conversion_sfinae<
    std::enable_if_t<can_initialize_range<To, From...>()>,
    To,
    From...> : range_initialization<To, From...>
{};

} // namespace ac
