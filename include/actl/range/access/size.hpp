// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/meta/has_member.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac::ranges {

AC_DEFINE_HAS_MEMBER_F(size)

namespace impl {

struct size_f
{
    template <
        class Range,
        enable_int_if<static_size_v<Range> != dynamic_size> = 0>
    constexpr constant<static_size_v<Range>> operator()(Range&&) const noexcept
    {
        return {};
    }

    template <
        class Range,
        enable_int_if<static_size_v<Range> == dynamic_size> = 0>
    constexpr auto operator()(Range&& range) const
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(range.size())

    template <
        class Range,
        enable_int_if<
            static_size_v<Range> == dynamic_size &&
            !has_member_f_size_v<Range>> = 0>
    constexpr auto operator()(Range&& range) const
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(size(range))
};

} // namespace impl

/// Replacement for std::size with the following benefits:
/// - support for user-specified size function that can be found by ADL;
/// - correct noexcept propagation in case range.size() is available.
inline constexpr impl::size_f size;

} // namespace ac::ranges
