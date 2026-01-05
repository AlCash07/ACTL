// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/traits/nothrow.hpp>

namespace ac::ranges {

template<std::input_or_output_iterator OutIter, Range SourceRange>
constexpr OutIter copy(OutIter target, SourceRange const& source) noexcept(
    is_nothrow_iterable_v<SourceRange const>&& noexcept(
        ++target, *target = *ranges::begin(source)
    )
) {
    auto first = ranges::begin(source);
    auto last = ranges::end(source);
    for (; first != last; ++first, (void)++target)
        *target = *first;
    return target;
}

template<Range TargetRange, Range SourceRange>
    requires have_matching_static_sizes_v<TargetRange, SourceRange>
constexpr void copy(TargetRange&& target, SourceRange const& source) noexcept(
    AC_ASSERT_IS_NOEXCEPT() && noexcept(copy(ranges::begin(target), source))
) {
    // TODO: refactor this for the case when size computation isn't cheap.
    AC_ASSERT(ranges::size(target) == ranges::size(source));
    copy(ranges::begin(target), source);
}

} // namespace ac::ranges
