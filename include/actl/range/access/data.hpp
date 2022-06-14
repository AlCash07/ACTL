// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac::ranges {

template <class T, class = void>
struct has_member_data : std::false_type
{};

template <class T>
struct has_member_data<T, void_t<decltype(std::declval<T>().data())>>
    : std::true_type
{};

struct data_f
{
    template <class T, size_t N>
    constexpr T* operator()(T (&array)[N]) const noexcept
    {
        return array;
    }

    template <class Range, enable_int_if<has_member_data<Range>::value> = 0>
    constexpr auto operator()(Range&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(range.data())

    template <class Range, enable_int_if<!has_member_data<Range>::value> = 0>
    constexpr auto operator()(Range&& range) const
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(data(range))
};
/// Replacement for std::data with the following benefits:
/// - support for user-specified data function that can be found by ADL;
/// - correct noexcept propagation in case range.data() is available.
inline constexpr data_f data;

} // namespace ac::ranges
