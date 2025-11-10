// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/interface/random_access_range_interface.hpp>

namespace ac {

template<typename Derived>
class contiguous_range_interface
    : public random_access_range_interface<
          contiguous_range_interface<Derived>> {
    constexpr Derived const& derived() const noexcept {
        return static_cast<Derived const&>(*this);
    }

public:
    constexpr auto begin() const AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        derived().data()
    )

    constexpr auto end() const AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        begin() + derived().size()
    )
};

} // namespace ac
