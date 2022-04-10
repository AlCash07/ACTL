// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/interface/random_access_range_interface.hpp>

namespace ac {

template <class Range, class Types>
class contiguous_range_interface
    : public random_access_range_interface<
          contiguous_range_interface<Range, Types>,
          Types>
{
    constexpr const Range& derived() const noexcept
    {
        return static_cast<const Range&>(*this);
    }

public:
    constexpr decltype(auto) begin() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().data())

    constexpr decltype(auto) end() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(begin() + derived().size())
};

} // namespace ac
