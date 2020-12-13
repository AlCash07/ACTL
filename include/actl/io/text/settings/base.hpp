// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/types.hpp>

namespace ac::io {

// Integer and real numbers base.
class base_t {
public:
    struct is_manipulator;

    explicit constexpr base_t() = default;

    template <class T>
    explicit constexpr base_t(T x) : value{static_cast<uint8_t>(x)} {
        ACTL_ASSERT(x == 0 || 1 < x && x <= 36);
    }

    base_t& operator=(index x) {
        return *this = base_t{x};
    }

    constexpr operator uint8_t() const {
        return value;
    }

private:
    uint8_t value = 10;
};

constexpr base_t dec{10};
constexpr base_t hex{16};
constexpr base_t oct{8};

}  // namespace ac::io