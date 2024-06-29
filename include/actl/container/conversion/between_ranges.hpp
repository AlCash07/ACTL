// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/convert_to.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/operation/copy.hpp>
#include <actl/range/traits/strict_range.hpp>

namespace ac {

template <class To, class From>
struct range_construction_from_iterators {
    using from_iter = range_iterator_t<From const>;

    static constexpr bool value =
        std::is_constructible_v<To, from_iter, from_iter>;

    static constexpr To convert(From const& x)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(To{ranges::begin(x), ranges::end(x)})
};

template <class To, class From>
struct range_construct_and_copy {
    static constexpr bool value = static_size_v<To> != dynamic_size &&
                                  std::is_default_constructible_v<To> &&
                                  std::is_assignable_v<
                                      range_reference_t<To>,
                                      range_reference_t<From const>>;

    static constexpr To convert(From const& x) noexcept(
        std::is_nothrow_default_constructible_v<To>&& noexcept(
            ranges::copy(std::declval<To>(), x)
        )
    ) {
        To result{};
        ranges::copy(result, x);
        return result;
    }
};

template <class To, class From>
struct ranges_conversion
    : std::conditional_t<
          range_construction_from_iterators<To, From>::value,
          range_construction_from_iterators<To, From>,
          range_construct_and_copy<To, From>> {};

template <class To, class From>
constexpr bool can_convert_as_ranges() noexcept {
    // We check for StrictRange, because we don't want to
    // miss additional type checking enabled by the tuple.
    if constexpr (StrictRange<To> && StrictRange<From> && have_matching_static_sizes_v<To, From>)
        return ranges_conversion<To, From>::value;
    else
        return false;
}

template <class To, class From>
    requires(can_convert_as_ranges<To, From>())
struct conversion<To, From> : ranges_conversion<To, From> {};

} // namespace ac
