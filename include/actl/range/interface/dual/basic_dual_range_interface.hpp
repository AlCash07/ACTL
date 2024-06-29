// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/interface/basic_range_interface.hpp>

namespace ac {

template <class Derived>
class basic_non_const_range_interface
{
public:
    constexpr auto cbegin() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().begin())

    constexpr auto cend() const AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().end())

    constexpr auto empty()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().begin() == derived().end())

    explicit constexpr operator bool() AC_DEDUCE_NOEXCEPT_AND_RETURN(!empty())

    constexpr decltype(auto) front() //
        noexcept(AC_ASSERT_IS_NOEXCEPT() && noexcept(*derived().begin()))
    {
        AC_ASSERT(!empty());
        return *derived().begin();
    }

protected:
    constexpr Derived& derived() noexcept
    {
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& derived() const noexcept
    {
        return static_cast<Derived const&>(*this);
    }
};

template <class Derived, class Types>
class basic_dual_range_interface
    : public range_interface<Derived, Types>
    , public basic_non_const_range_interface<Derived, Types>
{};

} // namespace ac
