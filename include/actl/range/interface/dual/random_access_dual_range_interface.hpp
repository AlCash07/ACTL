// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/interface/dual/bidirectional_dual_range_interface.hpp>
#include <actl/range/interface/random_access_range_interface.hpp>

namespace ac {

template <class Derived, class Types>
class random_access_non_const_range_interface
    : public bidirectional_non_const_range_interface<Derived, Types>
{
    using base_t = bidirectional_non_const_range_interface<Derived, Types>;

public:
    using typename base_t::difference_type;
    using typename base_t::size_type;

    constexpr decltype(auto) operator[](size_type n) //
        noexcept(AC_ASSERT_IS_NOEXCEPT() && noexcept(
            this->derived().begin()[static_cast<difference_type>(n)]))
    {
        AC_ASSERT(0 <= n && n < size());
        return this->derived().begin()[static_cast<difference_type>(n)];
    }

    constexpr auto size() AC_DEDUCE_NOEXCEPT_AND_RETURN(
        static_cast<size_type>(this->derived().end() - this->derived().begin()))
};

template <class Derived, class Types>
class random_access_dual_range_interface
    : public random_access_range_interface<Derived, Types>
    , public random_access_non_const_range_interface<Derived, Types>
{};

} // namespace ac
