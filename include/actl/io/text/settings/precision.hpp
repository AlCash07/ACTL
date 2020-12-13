// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/types.hpp>

namespace ac::io {

// Number of digits after the decimal point.
class precision_t {
public:
    explicit constexpr precision_t() = default;

    explicit constexpr precision_t(index x) : value{x} {
        ACTL_ASSERT(x >= 0);
    }

    precision_t& operator=(index x) {
        return *this = precision_t{x};
    }

    constexpr operator index() const {
        return value;
    }

private:
    index value = 6;
};

}  // namespace ac::io
