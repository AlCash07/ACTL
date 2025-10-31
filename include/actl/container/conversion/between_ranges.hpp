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

template<typename Target, typename Source>
struct range_construction_from_iterators {
    using source_iter = range_iterator_t<Source const>;

    static constexpr bool value =
        std::is_constructible_v<Target, source_iter, source_iter>;

    static constexpr Target convert(Source const& source)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(Target{
            ranges::begin(source), ranges::end(source)
        })
};

template<typename Target, typename Source>
struct range_construct_and_copy {
    static constexpr bool value = static_size_v<Target> != dynamic_size &&
                                  std::is_default_constructible_v<Target> &&
                                  std::is_assignable_v<
                                      range_reference_t<Target>,
                                      range_reference_t<Source const>>;

    static constexpr Target convert(Source const& source) noexcept(
        std::is_nothrow_default_constructible_v<Target>&& noexcept(
            ranges::copy(std::declval<Target>(), source)
        )
    ) {
        Target output{};
        ranges::copy(output, source);
        return output;
    }
};

template<typename Target, typename Source>
struct ranges_conversion
    : std::conditional_t<
          range_construction_from_iterators<Target, Source>::value,
          range_construction_from_iterators<Target, Source>,
          range_construct_and_copy<Target, Source>> {};

template<typename Target, typename Source>
constexpr bool can_convert_as_ranges() noexcept {
    // We check for StrictRange, because we don't want to
    // miss additional type checking enabled by the tuple.
    if constexpr (StrictRange<Target> && StrictRange<Source> && have_matching_static_sizes_v<Target, Source>)
        return ranges_conversion<Target, Source>::value;
    else
        return false;
}

template<typename Target, typename Source>
    requires(can_convert_as_ranges<Target, Source>())
struct conversion<Target, Source> : ranges_conversion<Target, Source> {};

} // namespace ac
