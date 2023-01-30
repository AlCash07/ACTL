// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <cstddef> // for size_t
#include <iterator>

namespace ac::ranges {

namespace impl {

template <class T>
concept has_member_begin = requires(T& t) {
                               {
                                   t.begin()
                                   } -> std::input_or_output_iterator;
                           };

template <class T>
concept has_non_member_begin = requires(T& t) {
                                   {
                                       begin(t)
                                       } -> std::input_or_output_iterator;
                               };

struct begin_f
{
    template <class T, size_t N>
    constexpr T* operator()(T (&array)[N]) const noexcept
    {
        return array;
    }

    constexpr auto operator()(has_member_begin auto&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(range.begin())

    template <class R>
        requires(!has_member_begin<R> && has_non_member_begin<R>)
    constexpr auto operator()(R&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(begin(range))
};

} // namespace impl

/// Replacement for std::begin with the following benefits:
/// - support for user-specified begin function that can be found by ADL;
/// - correct noexcept propagation in case range.begin() is available.
inline constexpr impl::begin_f begin;

} // namespace ac::ranges

namespace ac {

// This declaration is here because it's needed for ranges::end.
template <class R>
using range_iterator_t = decltype(ranges::begin(std::declval<R&>()));

} // namespace ac
