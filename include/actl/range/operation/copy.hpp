// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/traits/category.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/access/all.hpp>

namespace ac::ranges {

template <class OutIter, class Range, enable_int_if<is_iterator_v<OutIter>> = 0>
constexpr OutIter copy(OutIter dst, Range const& src)
{
    auto first = ranges::begin(src);
    auto last = ranges::end(src);
    for (; first != last; ++first, (void)++dst)
        *dst = *first;
    return dst;
}

template <
    class DstRange,
    class SrcRange,
    enable_int_if<
        is_range_v<DstRange> &&
        have_matching_static_sizes_v<DstRange, SrcRange>> = 0>
constexpr void copy(DstRange&& dst, SrcRange const& src)
{
    // TODO: refactor this for the case when size computation isn't cheap.
    AC_ASSERT(ranges::size(dst) == ranges::size(src));
    copy(ranges::begin(dst), src);
}

} // namespace ac::ranges
