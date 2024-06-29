// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/interface/basic_range_interface.hpp>

namespace ac {

template<class Derived>
class bidirectional_range_interface : public basic_range_interface<Derived> {
public:
    // decltype(auto) because a reference can be returned here.
    constexpr decltype(auto) back() const
        noexcept(AC_ASSERT_IS_NOEXCEPT() && noexcept(*--this->derived().end())
        ) {
        AC_ASSERT(!this->empty());
        auto last = this->derived().end();
        return *--last;
    }
};

} // namespace ac
