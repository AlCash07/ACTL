// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/interface/bidirectional_range_interface.hpp>

namespace ac {

template <class Derived, class Types>
class random_access_range_interface
    : public bidirectional_range_interface<Derived, Types>
{
    using base_t = bidirectional_range_interface<Derived, Types>;
    using base_t::derived;

public:
    using typename base_t::difference_type;
    using typename base_t::size_type;

    constexpr decltype(auto) operator[](size_type n) const
        noexcept(ACTL_ASSERT_IS_NOEXCEPT() && noexcept(
            derived().begin()[static_cast<difference_type>(n)]))
    {
        ACTL_ASSERT(0 <= n && n < size());
        return derived().begin()[static_cast<difference_type>(n)];
    }

    constexpr auto size() const AC_DEDUCE_NOEXCEPT_AND_RETURN(
        static_cast<size_type>(derived().end() - derived().begin()))
};

template <class Range, class Types>
struct range_interface_selector<Range, Types, std::random_access_iterator_tag>
{
    using type = random_access_range_interface<Range, Types>;
};

} // namespace ac
