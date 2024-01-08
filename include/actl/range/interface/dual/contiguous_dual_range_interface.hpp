// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/interface/contiguous_range_interface.hpp>
#include <actl/range/interface/dual/random_access_dual_range_interface.hpp>

namespace ac {

template <class Range>
class contiguous_non_const_range_interface
    : public random_access_non_const_range_interface<
          contiguous_non_const_range_interface<Range>>
{
    constexpr Range& derived() noexcept
    {
        return static_cast<Range&>(*this);
    }

public:
    constexpr auto begin() AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().data())

    constexpr auto end()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(begin() + derived().size())
};

template <class Derived>
class contiguous_dual_range_interface
    : public contiguous_range_interface<Derived>
    , public contiguous_non_const_range_interface<Derived>
{};

} // namespace ac
