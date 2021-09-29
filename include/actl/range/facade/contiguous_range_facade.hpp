// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/facade/range_facade.hpp>

namespace ac {

#define ENABLE_IF_HAS_CONST \
    template <class U = Types, class = typename U::const_iterator>

template <class Range, class Types>
class contiguous_range_facade
    : public range_facade<contiguous_range_facade<Range, Types>, Types>
{
    constexpr Range& derived() noexcept
    {
        return static_cast<Range&>(*this);
    }

    constexpr const Range& derived() const noexcept
    {
        return static_cast<const Range&>(*this);
    }

public:
    constexpr decltype(auto) begin() const noexcept(noexcept(derived().data()))
    {
        return derived().data();
    }

    ENABLE_IF_HAS_CONST constexpr decltype(auto) begin() noexcept(
        noexcept(derived().data()))
    {
        return derived().data();
    }

    constexpr decltype(auto) end() const
        noexcept(noexcept(begin() + derived().size()))
    {
        return begin() + derived().size();
    }

    ENABLE_IF_HAS_CONST constexpr decltype(auto) end() noexcept(
        noexcept(begin() + derived().size()))
    {
        return begin() + derived().size();
    }
};

#undef ENABLE_IF_HAS_CONST

} // namespace ac
