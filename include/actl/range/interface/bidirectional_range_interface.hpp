// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/interface/basic_range_interface.hpp>

namespace ac {

template <class Derived, class Types>
class bidirectional_range_interface
    : public basic_range_interface<Derived, Types>
{
protected:
    using basic_range_interface<Derived, Types>::derived;

public:
    constexpr decltype(auto) back() const
        noexcept(ACTL_ASSERT_IS_NOEXCEPT() && noexcept(*--derived().end()))
    {
        ACTL_ASSERT(!this->empty());
        auto last = derived().end();
        return *--last;
    }
};

template <class Range, class Types>
struct range_interface_selector<Range, Types, std::bidirectional_iterator_tag>
{
    using type = bidirectional_range_interface<Range, Types>;
};

} // namespace ac
