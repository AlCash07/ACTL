// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <cstddef> // for size_t

namespace ac::ranges {

namespace impl {

// TODO: check that data returns a pointer.
template <class T>
concept has_member_data = requires(T& t) {
                              {
                                  t.data()
                              };
                          };

template <class T>
concept has_non_member_data = requires(T& t) {
                                  {
                                      data(t)
                                  };
                              };

struct data_f
{
    template <class T, size_t N>
    constexpr T* operator()(T (&array)[N]) const noexcept
    {
        return array;
    }

    constexpr auto operator()(has_member_data auto&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(range.data())

    template <class R>
        requires(!has_member_data<R> && has_non_member_data<R>)
    constexpr auto operator()(R&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(data(range))
};

} // namespace impl

/// Replacement for std::data with the following benefits:
/// - support for user-specified data function that can be found by ADL;
/// - correct noexcept propagation in case range.data() is available.
inline constexpr impl::data_f data;

} // namespace ac::ranges
