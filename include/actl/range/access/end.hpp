// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/meta/has_member.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac::ranges {

AC_DEFINE_HAS_MEMBER_F(end)

struct end_f
{
    template <class T, size_t N>
    constexpr T* operator()(T (&array)[N]) const noexcept
    {
        return array + N;
    }

    template <class Range>
    constexpr auto operator()(Range&& range) const
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(range.end())

    template <class Range, enable_int_if<!has_member_f_end_v<Range>> = 0>
    constexpr auto operator()(Range&& range) const
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(end(range))
};
/// Replacement for std::end with the following benefits:
/// - support for user-specified end function that can be found by ADL;
/// - correct noexcept propagation in case range.end() is available.
inline constexpr end_f end;

} // namespace ac::ranges
