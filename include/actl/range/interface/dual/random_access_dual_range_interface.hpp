// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/interface/dual/bidirectional_dual_range_interface.hpp>
#include <actl/range/interface/random_access_range_interface.hpp>

namespace ac {

template<typename Derived>
class random_access_non_const_range_interface
    : public bidirectional_non_const_range_interface<Derived> {
public:
    template<typename Difference>
    constexpr decltype(auto) operator[](Difference n) //
        noexcept(AC_ASSERT_IS_NOEXCEPT() && noexcept(this->derived().begin()[n])
        ) {
        AC_ASSERT(0 <= n && n < size());
        return this->derived().begin()[n];
    }

    constexpr auto size() AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        to_unsigned(this->derived().end() - this->derived().begin())
    )
};

template<typename Derived>
class random_access_dual_range_interface
    : public random_access_range_interface<Derived>
    , public random_access_non_const_range_interface<Derived> {};

} // namespace ac
