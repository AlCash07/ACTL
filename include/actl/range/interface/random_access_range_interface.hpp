// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/numeric/signedness.hpp>
#include <actl/range/interface/bidirectional_range_interface.hpp>

namespace ac {

template <class Derived>
class random_access_range_interface
    : public bidirectional_range_interface<Derived>
{
public:
    template <class Difference>
    constexpr decltype(auto) operator[](Difference i) const noexcept(
        AC_ASSERT_IS_NOEXCEPT() && noexcept(this->derived().begin()[i]))
    {
        AC_ASSERT(0 <= i && i < size());
        return this->derived().begin()[i];
    }

    constexpr auto size() const AC_DEDUCE_NOEXCEPT_AND_RETURN(
        to_unsigned(this->derived().end() - this->derived().begin()))
};

} // namespace ac
