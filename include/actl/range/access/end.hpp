// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/access/begin.hpp>

namespace ac::ranges {

namespace impl {

template<class T>
concept has_member_end = requires(T& t) {
    { t.end() } -> std::sentinel_for<range_iterator_t<T>>;
};

template<class T>
concept has_non_member_end = requires(T& t) {
    { end(t) } -> std::sentinel_for<range_iterator_t<T>>;
};

struct end_f {
    template<class T, size_t N>
    constexpr T* operator()(T (&array)[N]) const noexcept {
        return array + N;
    }

    template<has_member_end R>
    constexpr auto operator()(R&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(range.end())

    template<class R>
        requires(!has_member_end<R> && has_non_member_end<R>)
    constexpr auto operator()(R&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(end(range))
};

} // namespace impl

/// Replacement for std::end with the following benefits:
/// - support for user-specified end function that can be found by ADL;
/// - correct noexcept propagation in case range.end() is available.
inline constexpr impl::end_f end;

} // namespace ac::ranges
