// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/interface/bidirectional_range_interface.hpp>
#include <actl/range/interface/dual/basic_dual_range_interface.hpp>

namespace ac {

template <class Derived, class Types>
class bidirectional_non_const_range_interface
    : public basic_non_const_range_interface<Derived, Types>
{
public:
    constexpr decltype(auto) back() //
        noexcept(AC_ASSERT_IS_NOEXCEPT() && noexcept(*--this->derived().end()))
    {
        AC_ASSERT(!this->empty());
        auto last = this->derived().end();
        return *--last;
    }
};

template <class Derived, class Types>
class bidirectional_dual_range_interface
    : public bidirectional_range_interface<Derived, Types>
    , public bidirectional_non_const_range_interface<Derived, Types>
{};

template <class Range, class Types>
struct dual_range_interface_selector<
    Range,
    Types,
    std::bidirectional_iterator_tag>
{
    using type = bidirectional_range_interface<Range, Types>;
};

} // namespace ac
