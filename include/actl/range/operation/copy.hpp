// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/traits/nesting_depth.hpp>
#include <actl/range/traits/nothrow.hpp>

namespace ac::ranges {

template<std::input_or_output_iterator OutIter, Range SrcRange>
constexpr OutIter copy(OutIter dst, SrcRange const& src) noexcept(
    is_nothrow_iterable_v<SrcRange const>&& noexcept(
        ++dst, *dst = *ranges::begin(src)
    )
) {
    auto first = ranges::begin(src);
    auto last = ranges::end(src);
    for (; first != last; ++first, (void)++dst)
        *dst = *first;
    return dst;
}

template<Range DstRange, Range SrcRange>
    requires have_matching_static_sizes_v<DstRange, SrcRange>
constexpr void copy(DstRange&& dst, SrcRange const& src) noexcept(
    AC_ASSERT_IS_NOEXCEPT() && noexcept(copy(ranges::begin(dst), src))
) {
    // TODO: refactor this for the case when size computation isn't cheap.
    AC_ASSERT(ranges::size(dst) == ranges::size(src));
    copy(ranges::begin(dst), src);
}

} // namespace ac::ranges
