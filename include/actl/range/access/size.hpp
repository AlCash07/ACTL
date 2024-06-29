// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/noexcept/deduce_noexcept_and_return.hpp>
#include <actl/meta/static_size.hpp>

namespace ac::ranges {

namespace impl {

// TODO: check that size is integer-like.
template <class T>
concept has_member_size = requires(T& t) {
    { t.size() };
};

template <class T>
concept has_non_member_size = requires(T& t) {
    { size(t) };
};

struct size_f {
    // We always return `ac::constant` when the size is a static constant,
    // even for types like `std::array` where size() returns a regular integer.
    template <HasStaticSize R>
    constexpr constant<static_size_v<R>> operator()(R&&) const noexcept {
        return {};
    }

    template <HasDynamicSize R>
        requires has_member_size<R>
    constexpr decltype(auto) operator()(R&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(range.size())

    template <HasDynamicSize R>
        requires(!has_member_size<R> && has_non_member_size<R>)
    constexpr decltype(auto) operator()(R&& range) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(size(range))
};

} // namespace impl

/// Replacement for std::size with the following benefits:
/// - support for user-specified size function that can be found by ADL;
/// - correct noexcept propagation in case range.size() is available.
inline constexpr impl::size_f size;

} // namespace ac::ranges
